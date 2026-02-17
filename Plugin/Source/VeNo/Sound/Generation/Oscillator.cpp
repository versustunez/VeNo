#include "VUtils/Math.h"
#include "VeNo/Utils/LookupTable.h"

#include <VeNo/Core/Instance.h>
#include <VeNo/Sound/DSP/Detune/Detune.h>
#include <VeNo/Sound/Generation/Oscillator.h>
#include <VeNo/Utils/Random.h>

#if USE_AVX2
#include <immintrin.h>
#endif

namespace VeNo::Audio {
#if USE_AVX2
inline __m128 hermite4_ps(__m128 f, __m128 xm1, __m128 x0, __m128 x1,
                          __m128 x2) {
  const __m128 half = _mm_set1_ps(0.5f);

  __m128 c = _mm_mul_ps(_mm_sub_ps(x1, xm1), half);
  __m128 v = _mm_sub_ps(x0, x1);
  __m128 w = _mm_add_ps(c, v);
  __m128 a = _mm_add_ps(_mm_add_ps(w, v), _mm_mul_ps(_mm_sub_ps(x2, x0), half));
  __m128 bneg = _mm_add_ps(w, a);

  __m128 r = _mm_sub_ps(_mm_mul_ps(a, f), bneg);
  r = _mm_add_ps(_mm_mul_ps(r, f), c);
  r = _mm_add_ps(_mm_mul_ps(r, f), x0);
  return r;
}

inline __m128 renderVoiceSIMD4(SingleVoiceData &voice, DetuneState &state,
                               float inc, const Wave &table, int baseIdx) {
  const int mask = table.Length - 1;

  __m128 lookup = _mm_loadu_ps(&state.lookup[baseIdx]);
  __m128 phase = _mm_loadu_ps(&voice.phaseOffset[baseIdx]);

  __m128 incv = _mm_mul_ps(_mm_set1_ps(inc), lookup);
  phase = _mm_add_ps(phase, incv);

  __m128 one = _mm_set1_ps(1.0f);
  __m128 wrapMask = _mm_cmpge_ps(phase, one);
  phase = _mm_sub_ps(phase, _mm_and_ps(wrapMask, one));

  _mm_storeu_ps(&voice.phaseOffset[baseIdx], phase);

  __m128 pos = _mm_mul_ps(phase, _mm_set1_ps((float)table.Length));

  __m128i i = _mm_cvttps_epi32(pos);
  __m128 f = _mm_sub_ps(pos, _mm_cvtepi32_ps(i));

  __m128i maskv = _mm_set1_epi32(mask);

  __m128i im1 = _mm_and_si128(_mm_sub_epi32(i, _mm_set1_epi32(1)), maskv);
  __m128i i0 = _mm_and_si128(i, maskv);
  __m128i i1 = _mm_and_si128(_mm_add_epi32(i, _mm_set1_epi32(1)), maskv);
  __m128i i2 = _mm_and_si128(_mm_add_epi32(i, _mm_set1_epi32(2)), maskv);

  const float *base = table.Data;

  __m128 xm1 = _mm_i32gather_ps(base, im1, 4);
  __m128 x0 = _mm_i32gather_ps(base, i0, 4);
  __m128 x1 = _mm_i32gather_ps(base, i1, 4);
  __m128 x2 = _mm_i32gather_ps(base, i2, 4);

  return hermite4_ps(f, xm1, x0, x1, x2);
}
#endif

void Oscillator::setupModulatorState(OscillatorModulatorState &osc,
                                     size_t instanceId, size_t index) {
  // get all audio parameter references needed for the oscillator
  auto &state = osc;
  auto *instance = Core::Instance::get(instanceId);
  auto *handler = instance->handler.get();
  state.lib = &WaveLib::Get();
  state.active = handler->getOscParameter("active", index);
  state.voices = handler->getOscParameter("voices", index);
  state.semitones = handler->getOscModulateParameter("semitones", index);
  state.cents = handler->getOscModulateParameter("cents", index);
  state.level = handler->getOscModulateParameter("level", index);
  state.panning = handler->getOscModulateParameter("panning", index);
  state.detuneAmount = handler->getOscModulateParameter("detune_amount", index);
  state.detuneDense = handler->getOscModulateParameter("detune_dense", index);
  state.detuneMode = handler->getOscParameter("detune_mode", index);
  state.phase = handler->getOscModulateParameter("phase", index);
  state.randomPhase = handler->getOscParameter("random_phase", index);
  state.retrigger = handler->getOscParameter("retrigger", index);
  state.stereo = handler->getOscModulateParameter("stereo", index);
  state.wave = handler->getOscParameter("wave", index);
  state.pitchWheel = handler->getParameter("pitch__wheel");
  state.pitchBendUp = handler->getParameter("pitchbend__up");
  state.pitchBendDown = handler->getParameter("pitchbend__down");
  DBGN("Created Oscillator State for: {}", index);
}

bool Oscillator::prepare(OscillatorData &osc,
                         OscillatorModulatorState &modulator_state) {
  // copy state
  auto &state = osc.state;
  state.active = modulator_state.active->getBool();
  state.randomPhase = modulator_state.randomPhase->getBool();
  state.retrigger = modulator_state.retrigger->getBool();
  state.voices = modulator_state.voices->getInt();
  state.detuneMode = modulator_state.detuneMode->getInt();
  state.wave = modulator_state.wave->getInt();
  modulator_state.detuneDense->copyInto(state.detuneDense);
  modulator_state.semitones->copyInto(state.semitones);
  modulator_state.cents->copyInto(state.cents);
  modulator_state.level->copyInto(state.level);
  modulator_state.panning->copyInto(state.panning);
  modulator_state.detuneAmount->copyInto(state.detuneAmount);
  modulator_state.phase->copyInto(state.phase);
  modulator_state.stereo->copyInto(state.stereo);
  state.lib = modulator_state.lib;

  osc.group = osc.state.lib->GetWaveTable(osc.state.wave - 1);

  for (size_t i = 0; i < MAX_VOICES; ++i) {
    Detune::update(osc.detuneState[i], osc.state, i);
    Widener::Update(osc, i);
  }
  return false;
}
bool Oscillator::process(OscillatorData &osc, SingleVoiceData &voice,
                         float currentNote, double sR) {
  if (!osc.state.active)
    return false;
  // update voice data
  updateFrequency(osc, voice, currentNote);
  render(osc, voice, sR);
  return true;
}
bool Oscillator::finish(OscillatorData &, SingleVoiceData &) {
  // after hook maybe to shut down something or more :) // its maybe to-be
  // removed
  return false;
}
void Oscillator::updateFrequency(OscillatorData &osc, SingleVoiceData &voice,
                                 float currentNote) {
  // Create Lookup Table for Midi Lookups
  static Utils::LookupTable<130> s_MidiLookup{[](float *elements, size_t size) {
    for (size_t i = 0; i < size; ++i) {
      elements[i] = (float)std::exp((i - 69.0) * std::log(2) / 12) * 440.0f;
    }
  }};
  auto &state = osc.state;
  auto wheelPos = state.pitchWheel;
  double pitchPos = wheelPos >= 0 ? state.pitchBendUp : state.pitchBendDown;
  double pitchBend = pitchPos * wheelPos;
  auto semitones = (state.semitones)[voice.id];
  auto cents = (state.cents)[voice.id] / 100.0;
  double orgMid =
      std::clamp(currentNote + semitones + cents + pitchBend, 0.0, 127.0);
  int midi = (int)(orgMid);
  double diff = orgMid - midi;
  voice.frequency =
      VUtils::Math::lerp(s_MidiLookup[midi], s_MidiLookup[midi + 1], diff);
}

void Oscillator::render(OscillatorData &osc, SingleVoiceData &voice,
                        double sR) {
  const int voiceCount = osc.state.voices;
  const int index = voice.id;

  if (voiceCount == 0) {
    voice.output.left = voice.output.right = 0;
    return;
  }

  const auto inc = static_cast<float>(voice.frequency / sR);

  size_t currentTable = 0;
  while ((currentTable < (osc.group->Length - 1) &&
          (inc >= osc.group->Waves[currentTable].TopFreq))) {
    ++currentTable;
  }

  auto &table = osc.group->Waves[currentTable];

  float left = 0.0f;
  float right = 0.0f;

  // Voice 0 (kept scalar — avoids SIMD setup cost)
  float v0 = renderVoice(voice, osc.detuneState[index], inc, table, 0);
  left += v0;
  right += v0;

  float dOut[2] = {0, 0};

#if USE_AVX2
  int i = 1;

  for (; i + 3 < voiceCount; i += 4) {
    __m128 r = renderVoiceSIMD4(voice, osc.detuneState[index], inc, table, i);

    alignas(16) float tmp[4];
    _mm_store_ps(tmp, r);

    dOut[(i + 0) & 1] += tmp[0];
    dOut[(i + 1) & 1] += tmp[1];
    dOut[(i + 2) & 1] += tmp[2];
    dOut[(i + 3) & 1] += tmp[3];
  }

  for (; i < voiceCount; ++i)
    dOut[i & 1] +=
        renderVoice(voice, osc.detuneState[index], inc, table, i);
#else
  for (int i = 1; i < voiceCount; ++i)
    dOut[i & 1] += renderVoice(voice, osc.detuneState[index], inc, table, i);
#endif

  const float amount = osc.state.detuneAmount[index];
  left += dOut[0] * amount;
  right += dOut[1] * amount;

  voice.output.left = left;
  voice.output.right = right;

  Widener::Apply(osc.widenerState[index], voice.output);
  Widener::ApplyPanning(osc.widenerState[index], voice.output);

  const float vol = osc.state.level[index];
  voice.output.left *= vol;
  voice.output.right *= vol;
}

inline float hermite(float frac_pos, float xm1, float x0, float x1, float x2) {
  const float c = (x1 - xm1) * 0.5f;
  const float v = x0 - x1;
  const float w = c + v;
  const float a = w + v + (x2 - x0) * 0.5f;
  const float b_neg = w + a;
  return ((((a * frac_pos) - b_neg) * frac_pos + c) * frac_pos + x0);
}

float Oscillator::renderVoice(SingleVoiceData &voice, DetuneState &state,
                              double inc, const Wave &table, int idx) {
  voice.phaseInc[idx] = (float)inc * state.lookup[idx];
  float phase = voice.phaseOffset[idx] + voice.phaseInc[idx];
  phase -= static_cast<float>(phase >= 1.0f);
  voice.phaseOffset[idx] = phase;

  // Scale to table
  const float pos = phase * static_cast<float>(table.Length);
  const int i = static_cast<int>(pos);
  const float f = pos - static_cast<float>(i);

  const int mask = table.Length - 1;
  const float xm1 = table.Data[(i - 1) & mask];
  const float x0 = table.Data[(i)&mask];
  const float x1 = table.Data[(i + 1) & mask];
  const float x2 = table.Data[(i + 2) & mask];

  return hermite(f, xm1, x0, x1, x2);
}

static Utils::Random s_Random{};
void Oscillator::prepareVoice(OscillatorData &osc, SingleVoiceData &voice) {
  if (!osc.state.retrigger)
    return;
  if (osc.state.randomPhase) {
    for (float &value : voice.phaseOffset) {
      value = static_cast<float>(s_Random.generate());
    }
  } else {
    for (float &value : voice.phaseOffset) {
      value = osc.state.phase[voice.id];
    }
  }
}
} // namespace VeNo::Audio
