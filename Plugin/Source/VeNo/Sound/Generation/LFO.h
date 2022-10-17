#pragma once

// Normally LFOs are Oscillators but inside VeNo LFOs have fewer Features.
// so we don't have to use the Oscillator Pipeline
#include "VeNo/Core/Parameter/Parameter.h"

#include <VeNo/Sound/Structs.h>

namespace VeNo::Audio {
struct LFOData {
  double Output{};
  double Phase{0};
  double FreqInc{0}; // already calculated based on SampleRate and Freq.
  Core::ModulateParameter *Frequency{nullptr};
  Core::Parameter *Active{nullptr};
  Core::Parameter *WaveForm{nullptr};
  int LastWaveForm{0};
  WaveTable *Table{nullptr};
};
} // namespace VeNo::Audio