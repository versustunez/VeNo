#include "VeNo/Core/Instance.h"
#include "VeNo/Core/Parameter/Handler.h"
#include <VeNo/Sound/Generation/Oscillator.h>
namespace VeNo::Audio
// @TODO Implement Oscillator
{
bool Oscillator::setup(OscillatorData &osc, size_t instanceId) {
  // get all audio parameter references needed for the oscillator
  auto &state = osc.state;
  auto *handler = Core::Instance::get(instanceId)->handler.get();
  state.active = handler->getOscParameter("active", osc.id);
  state.voices = handler->getOscParameter("voices", osc.id);
  state.semitones = handler->getOscParameter("semitones", osc.id);
  state.cents = handler->getOscParameter("cents", osc.id);
  state.level = handler->getOscParameter("level", osc.id);
  state.panning = handler->getOscParameter("panning", osc.id);
  state.detuneAmount = handler->getOscParameter("detune_amount", osc.id);
  state.detuneDense = handler->getOscParameter("detune_dense", osc.id);
  state.detuneMode = handler->getOscParameter("detune_mode", osc.id);
  state.phase = handler->getOscParameter("phase", osc.id);
  state.randomPhase = handler->getOscParameter("random_phase", osc.id);
  state.stereo = handler->getOscParameter("stereo", osc.id);
  state.wavePosition = handler->getOscParameter("wave_position", osc.id);
  state.attack = handler->getOscParameter("attack", osc.id);
  state.decay = handler->getOscParameter("decay", osc.id);
  state.sustain = handler->getOscParameter("sustain", osc.id);
  state.release = handler->getOscParameter("release", osc.id);
  return true;
}

bool Oscillator::prepare(OscillatorData &osc, SingleVoiceData &voice) {
  // the step we update all internal states for the oscillator-data like Detune
  // and more Time to implement preparing of all oscillators -> it also needs to
  // prepare all active voice frequencies
  return false;
}
bool Oscillator::update(OscillatorData &, SingleVoiceData &) {
  // update voice data
  return false;
}
bool Oscillator::finish(OscillatorData &, SingleVoiceData &) {
  // after hook maybe to shutdown something or more :) // its maybe to-be
  // removed
  return false;
}
} // namespace VeNo::Audio
