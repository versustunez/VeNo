#include "VUtils/Logging.h"

#include <VeNo/Sound/DSP/Detune/Detune.h>
#include <VeNo/Sound/DSP/Detune/Super.h>
#include <VeNo/Sound/DSP/Detune/VeNoX.h>

namespace VeNo::Audio {

void Detune::update(DetuneState & detune, OscillatorState & osc, int midi) {
  int voices = osc.voices->getInt();
  double amount = osc.detuneDense->getValue();
  // we are in the right state! skip
  if (voices == detune.lastVoices && amount == detune.lastDetune && detune.midiNote == midi)
    return;
  detune.lastVoices = voices;
  detune.lastDetune = amount;
  detune.midiNote = midi;
  auto mode = osc.detuneMode->getInt();
  switch (mode) {
  case DetuneModes::SUPER:
    SuperDetune::create(detune);
    break;
  case DetuneModes::VeNoX:
    VeNoXDetune::create(detune);
    break;
  default:
    WARN("Unimplemented Detune Mode");
    assert(true);
  }
}
}