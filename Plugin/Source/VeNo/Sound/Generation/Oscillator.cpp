#include "VUtils/Math.h"
#include "VeNo/Utils/LookupTable.h"

#include <VeNo/Core/Instance.h>
#include <VeNo/Sound/DSP/Detune/Detune.h>
#include <VeNo/Sound/Generation/Oscillator.h>
#include <VeNo/Utils/Random.h>
namespace VeNo::Audio {
void Oscillator::setupModulatorState(OscillatorModulatorState &osc, size_t instanceId, size_t index) {
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

bool Oscillator::prepare(OscillatorData &osc, OscillatorModulatorState &modulator_state) {
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
  double pitchPos = wheelPos >= 0 ? state.pitchBendUp
                                  : state.pitchBendDown;
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
  auto voiceCount = osc.state.voices;
  auto index = voice.id;
  if (voiceCount == 0) {
    voice.output.left = 0;
    voice.output.right = 0;
    return;
  }
  double inc = voice.frequency / sR;
  size_t currentTable = 0;
  while ((currentTable < (osc.group->Length - 1) &&
          (inc >= osc.group->Waves[currentTable].TopFreq))) {
    ++currentTable;
  }
  auto &table = osc.group->Waves[currentTable];
  auto output = renderVoice(voice, osc.detuneState[index], inc, table,
                            0); // first Voice because its special
  voice.output.left = output;
  voice.output.right = output;
  if (voiceCount > 1) {
    double dOut[2] = {0, 0};
    for (int i = 1; i < voiceCount; ++i) {
      dOut[i & 1] += renderVoice(voice, osc.detuneState[index], inc, table, i);
    }
    double amount = osc.state.detuneAmount[index];
    dOut[0] *= amount;
    dOut[1] *= amount;
    voice.output.left += dOut[0];
    voice.output.right += dOut[1];
  }
  Widener::Apply(osc.widenerState[index], voice.output);
  Widener::ApplyPanning(osc.widenerState[index], voice.output);
  double vol = osc.state.level[index];
  voice.output.left *= vol;
  voice.output.right *= vol;
}

double Oscillator::renderVoice(SingleVoiceData &voice, DetuneState &state,
                               double inc, const Wave &table, int idx) {
  voice.phaseInc[idx] = (float)inc * state.lookup[idx];
  voice.phaseOffset[idx] += voice.phaseInc[idx];
  voice.phaseOffset[idx] -= float(voice.phaseOffset[idx] >= 1.0);

  // LETS GO ;)
  double val = voice.phaseOffset[idx] * table.Length;
  int value = (int)val;
  int temp = value + 1;
  double sum = table.Data[value];
  double sum2 = table.Data[temp];

  double fraction = val - value;
  return VUtils::Math::lerp(sum, sum2, fraction);
}

static Utils::Random s_Random{};
void Oscillator::prepareVoice(OscillatorData &osc, SingleVoiceData &voice) {
  if (!osc.state.retrigger)
    return;
  if (osc.state.randomPhase) {
    for (float & value : voice.phaseOffset) {
      value = static_cast<float>(s_Random.generate());
    }
  } else {
    for (float & value : voice.phaseOffset) {
      value = osc.state.phase[voice.id];
    }
  }
}
} // namespace VeNo::Audio
