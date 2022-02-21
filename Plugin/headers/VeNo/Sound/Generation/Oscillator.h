#pragma once

#include "OscillatorStructs.h"

// Oscillator is a Stateless class!
namespace VeNo::Audio {
// not used by LFO because LFO is not the same!
class Oscillator {
public:
  static bool setup(OscillatorData &, size_t instanceId);
  static bool prepare(OscillatorData &);
  static bool process(OscillatorData &, SingleVoiceData &);
  static bool finish(OscillatorData &, SingleVoiceData &);

protected:
  Oscillator() = default;
};
} // namespace VeNo::Audio
