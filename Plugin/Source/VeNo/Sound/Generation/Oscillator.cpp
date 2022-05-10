#include "VUtils/Math.h"
#include "VeNo/Utils/LookupTable.h"

#include <VeNo/Core/Instance.h>
#include <VeNo/Sound/DSP/Detune/Detune.h>
#include <VeNo/Sound/Generation/Oscillator.h>
#include <VeNo/Utils/ProfileMacros.h>
#include <VeNo/Utils/Random.h>
namespace VeNo::Audio {
bool Oscillator::setup(OscillatorData &osc, size_t instanceId) {
  // get all audio parameter references needed for the oscillator
  auto &state = osc.state;
  auto *instance = Core::Instance::get(instanceId);
  auto *handler = instance->handler.get();
  state.lib = instance->waveHolder.generators[osc.id - 1].get();
  state.active = handler->getOscParameter("active", osc.id);
  state.voices = handler->getOscParameter("voices", osc.id);
  state.semitones = handler->getOscModulateParameter("semitones", osc.id);
  state.cents = handler->getOscModulateParameter("cents", osc.id);
  state.level = handler->getOscModulateParameter("level", osc.id);
  state.panning = handler->getOscModulateParameter("panning", osc.id);
  state.detuneAmount =
      handler->getOscModulateParameter("detune_amount", osc.id);
  state.detuneDense = handler->getOscModulateParameter("detune_dense", osc.id);
  state.detuneMode = handler->getOscParameter("detune_mode", osc.id);
  state.phase = handler->getOscModulateParameter("phase", osc.id);
  state.randomPhase = handler->getOscParameter("random_phase", osc.id);
  state.stereo = handler->getOscModulateParameter("stereo", osc.id);
  state.wavePosition = handler->getOscParameter("wave_position", osc.id);
  state.pitchWheel = handler->getParameter("pitch__wheel");
  state.pitchBendUp = handler->getParameter("pitchbend__up");
  state.pitchBendDown = handler->getParameter("pitchbend__down");
  DBGN("Created Oscillator State for: {}", osc.id);
  return true;
}

bool Oscillator::prepare(OscillatorData &osc) {
  osc.group = osc.state.lib->getGroup(osc.state.wavePosition->getValue());
  Detune::update(osc.detuneState, osc.state);
  return false;
}
bool Oscillator::process(OscillatorData &osc, SingleVoiceData &voice,
                         int currentNote, double sR) {
  if (!osc.state.active->getBool())
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
                                 int currentNote) {
  // Create Lookup Table for Midi Lookups
  static Utils::LookupTable<130> s_MidiLookup{[](float *elements, size_t size) {
    for (size_t i = 0; i < size; ++i) {
      elements[i] = (float)std::exp((i - 69.0) * std::log(2) / 12) * 440.0f;
    }
  }};
  auto &state = osc.state;
  auto wheelPos = state.pitchWheel->getValue();
  double pitchPos = wheelPos >= 0 ? state.pitchBendUp->getValue()
                                  : state.pitchBendDown->getValue();
  double pitchBend = pitchPos * wheelPos;
  auto semitones = (*state.semitones)[-1];
  auto cents = (*state.cents)[-1] / 100.0;
  double orgMid =
      std::clamp(currentNote + semitones + cents + pitchBend, 0.0, 127.0);
  int midi = (int)(orgMid);
  double diff = orgMid - midi;
  voice.frequency =
      VUtils::Math::lerp(s_MidiLookup[midi], s_MidiLookup[midi + 1], diff);
}

void Oscillator::render(OscillatorData &osc, SingleVoiceData &voice,
                        double sR) {
  auto voiceCount = osc.state.voices->getInt();
  if (voiceCount == 0) {
    voice.output.left = 0;
    voice.output.right = 0;
    return;
  }
  double inc = voice.frequency / sR;
  size_t currentTable = 0;
  while ((currentTable < (osc.group->len - 1) &&
          (inc >= osc.group->items[currentTable].freq))) {
    ++currentTable;
  }
  auto &table = osc.group->items[currentTable];
  auto output = renderVoice(voice, osc.detuneState, inc, table,
                            0); // first Voice because its special
  voice.output.left = output;
  voice.output.right = output;
  if (voiceCount > 1) {
    double dOut[2] = {0, 0};
    double detuneOut = 0;
    for (int i = 1; i < voiceCount; ++i) {
      output = renderVoice(voice, osc.detuneState, inc, table, i);
      dOut[i & 1] = output * 0.43333333;
      detuneOut += output;
    }
    double amount = osc.state.detuneAmount->getValue();
    dOut[0] *= amount;
    dOut[1] *= amount;
    detuneOut *= osc.state.detuneAmount->getValue();
    detuneOut /= (double)(voiceCount - 1);
    voice.output.left += detuneOut + dOut[0];
    voice.output.right += detuneOut + dOut[1];
  }

  double vol = osc.state.level->getVoice(voice.id);
  voice.output.left *= vol;
  voice.output.right *= vol;
}
float Oscillator::renderVoice(SingleVoiceData &voice, DetuneState &state,
                              double inc, const Wave &table, int idx) {
  auto &d = voice.unisonVoices[idx];
  d.phaseInc = (float)inc * state.lookup[idx];
  d.phaseOffset += d.phaseInc;
  d.phaseOffset -= float(d.phaseOffset >= 1.0);

  // LETS GO ;)
  double val = d.phaseOffset * (double)table.len;
  int value = (int)val;
  int temp = (int)val + 1;
  double sum = table.items[value];
  double sum2 = table.items[temp];

  double fraction = val - (double)value;
  double finalSum = sum + fraction * (sum2 - sum);
  return finalSum;
}

void Oscillator::prepareVoice(OscillatorData &osc, SingleVoiceData voice) {
  auto &random = Utils::Random::get();
  if (osc.state.randomPhase->getBool()) {
    for (auto &unisonVoice : voice.unisonVoices) {
      unisonVoice.phaseOffset = (float)random.generate();
    }
  } else {
    for (auto &unisonVoice : voice.unisonVoices) {
      unisonVoice.phaseOffset = (float)osc.state.phase->getValue();
    }
  }
}
} // namespace VeNo::Audio
