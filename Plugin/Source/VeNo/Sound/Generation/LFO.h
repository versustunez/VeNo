#pragma once

// Normally LFOs are Oscillators but inside VeNo LFOs have fewer Features.
// so we don't have to use the Oscillator Pipeline
#include <VeNo/Sound/Structs.h>

namespace VeNo::Audio {
struct LFOVoice {
  double output{0};
  double phase{0};
};
struct LFOData {
  Channel voiceData[MAX_VOICES]{};
  bool isActive{false};
  double freqInc{0}; // already calculated based on SampleRate and Freq.
};

class LFO {
private:
  LFO() = default;
};
}