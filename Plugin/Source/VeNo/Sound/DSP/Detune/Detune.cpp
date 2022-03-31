#include "VUtils/Logging.h"

#include <VeNo/Sound/DSP/Detune/Detune.h>
#include <VeNo/Sound/DSP/Detune/DetuneAlgos.h>

namespace VeNo::Audio {

void Detune::update(DetuneState &detune, OscillatorState &osc, int midi) {
  int voices = osc.voices->getInt();
  int mode = osc.detuneMode->getInt();
  double amount = osc.detuneDense->getValue();
  // we are in the right state! skip
  bool voicesChanged = voices != detune.lastVoices;
  bool amountChanged = amount != detune.lastDetune;
  bool modeChanged = mode != detune.lastMode;
  bool midiChanged = detune.midiNote != midi;
  bool detuneChanged = amountChanged || modeChanged || voicesChanged;
  if (!midiChanged && !detuneChanged)
    return;
  bool midiOnly = midiChanged && !detuneChanged;
  detune.lastVoices = voices;
  detune.lastDetune = (float)amount;
  detune.midiNote = midi;
  switch (mode) {
  case DetuneModes::SUPER: SuperDetune::create(detune, midiOnly); break;
  case DetuneModes::VeNoX: VeNoXDetune::create(detune, midiOnly); break;
  case DetuneModes::UP_SHIFT: UPShiftDetune::create(detune, midiOnly); break;
  case DetuneModes::EXPERIMENTAL:
    ExperimentalDetune::create(detune, midiOnly);
    break;
  default: WARN("Unimplemented Detune Mode"); assert(true);
  }
}
} // namespace VeNo::Audio