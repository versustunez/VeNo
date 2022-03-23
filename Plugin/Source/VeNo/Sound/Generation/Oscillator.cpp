#include "VeNo/Core/Instance.h"
#include "VeNo/Core/Parameter/Handler.h"

#include <VeNo/Sound/Generation/Oscillator.h>
namespace VeNo::Audio
// @TODO Implement Oscillator
{
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
  DBGN("Created Oscillator State for: %d", osc.id);
  return true;
}

bool Oscillator::prepare(OscillatorData &osc) {
  // the step we update all internal states for the oscillator-data like Detune
  osc.group = osc.state.lib->getGroup(osc.state.wavePosition->getValue());
  return false;
}
bool Oscillator::process(OscillatorData &osc, SingleVoiceData &voice,
                         int currentNote) {
  if (!osc.state.active->getBool())
    return false;
  // update voice data
  updateFrequency(osc, voice, currentNote);
  render(osc, voice);
  return true;
}
bool Oscillator::finish(OscillatorData &, SingleVoiceData &) {
  // after hook maybe to shut down something or more :) // its maybe to-be
  // removed
  return false;
}
void Oscillator::updateFrequency(OscillatorData &osc, SingleVoiceData &voice,
                                 int currentNote) {

  auto &state = osc.state;
  auto wheelPos = state.pitchWheel->getValue();
  double pitchPos = wheelPos >= 0 ? state.pitchBendUp->getValue()
                                  : state.pitchBendDown->getValue();
  double pitchBend = wheelPos == 0 ? 0.0 : pitchPos * wheelPos;
  auto semitones = state.semitones->getVoice(voice.id);
  auto cents = state.cents->getVoice(voice.id) / 100.0;
  double midi =
      std::clamp(currentNote + semitones + cents + pitchBend, 0.0, 127.0);
  voice.frequency = std::exp((midi - 69) * std::log(2) / 12) * 440.0f;
}

void Oscillator::render(OscillatorData &osc, SingleVoiceData &voice) {
  double sR = Core::Config::get().sampleRate;
  double inc = voice.frequency / sR;
  voice.output.left = 0;
  voice.output.right = 0;
  size_t currentTable = 0;
  while ((currentTable < (osc.group->len - 1) &&
          (inc >= osc.group->items[currentTable].freq)))
  {
    ++currentTable;
  }
  auto& table = osc.group->items[currentTable];
  for (int i = 0; i < voice.voices; ++i) {
    auto& d = voice.unisonVoices[i];
    d.phaseInc = (float)inc;
    d.phaseOffset += d.phaseInc;
    d.phaseOffset -= float(d.phaseOffset >= 1.0);

    // LETS GO ;)
    double val = d.phaseOffset * (double) table.len;
    int value = (int) val;
    int temp = (int) val + 1;
    double sum = table.items[value];
    double sum2 = table.items[temp];

    double fraction = val - (double) value;
    double finalSum = sum + fraction * (sum2 - sum);
    voice.output.left += finalSum;
    voice.output.right += finalSum;
  }

  double vol = osc.state.level->getVoice(voice.id);
  voice.output.left *= vol;
  voice.output.right *= vol;
}
} // namespace VeNo::Audio
