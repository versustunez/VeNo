#include "VUtils/Logging.h"

#include <VeNo/Sound/DSP/Detune/Detune.h>
#include <VeNo/Sound/DSP/Detune/DetuneAlgos.h>

namespace VeNo::Audio {

void Detune::update(DetuneState &detune, OscillatorState &osc) {
  int voices = osc.voices->getInt();
  int mode = osc.detuneMode->getInt();
  auto amount = (float)osc.detuneDense->getValue();
  // we are in the right state! skip
  bool voicesChanged = voices != detune.lastVoices;
  bool amountChanged = amount != detune.lastDetune;
  bool modeChanged = mode != detune.lastMode;
  if (!amountChanged && !modeChanged && !voicesChanged)
    return;
  detune.lastVoices = voices;
  detune.lastDetune = amount;
  switch (mode) {
  case DetuneModes::SUPER: SuperDetune::create(detune); break;
  case DetuneModes::VeNoX: VeNoXDetune::create(detune); break;
  case DetuneModes::UP_SHIFT: UPShiftDetune::create(detune); break;
  case DetuneModes::EXPERIMENTAL: ExperimentalDetune::create(detune); break;
  default: WARN("Unimplemented Detune Mode"); assert(true);
  }
}
} // namespace VeNo::Audio