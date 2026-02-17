#include "VUtils/Logging.h"

#include <VeNo/Sound/DSP/Detune/Detune.h>
#include <VeNo/Sound/DSP/Detune/DetuneAlgos.h>

namespace VeNo::Audio {

void Detune::update(DetuneState &detune, OptimizedOscillatorState &osc, size_t voice) {
  const int voices = osc.voices;
  const int mode = osc.detuneMode;
  const auto amount = osc.detuneDense[voice];
  // we are in the right state! skip
  const bool voicesChanged = voices != detune.lastVoices;
  const bool amountChanged = amount != detune.lastDetune;
  if (bool modeChanged = mode != detune.lastMode;
      !amountChanged && !modeChanged && !voicesChanged)
    return;
  detune.lastVoices = voices;
  detune.lastDetune = amount;
  detune.lastMode = mode;
  switch (mode) {
  case DetuneModes::SUPER: SuperDetune::create(detune); break;
  case DetuneModes::VeNoX: VeNoXDetune::create(detune); break;
  case DetuneModes::UP_SHIFT: UPShiftDetune::create(detune); break;
  case DetuneModes::EXPERIMENTAL: ExperimentalDetune::create(detune); break;
  default: WARN("Unimplemented Detune Mode"); assert(true);
  }
}
} // namespace VeNo::Audio