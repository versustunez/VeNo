#pragma once

#include "OscillatorStructs.h"

// Oscillator is a Stateless class!
namespace VeNo::Audio {
// not used by LFO because LFO is not the same!
class Oscillator {
public:
  static bool setup(OscillatorData &, size_t instanceId);
  static bool prepare(OscillatorData &);
  static bool process(OscillatorData &, SingleVoiceData &, float currentNote,
                      double sR);
  static bool finish(OscillatorData &, SingleVoiceData &);
  static void prepareVoice(OscillatorData &osc, SingleVoiceData voice);
  static double renderVoice(SingleVoiceData &voice, DetuneState &detune,
                           double inc, const Wave &table, int idx);

protected:
  static void updateFrequency(OscillatorData &, SingleVoiceData &,
                              float currentNote);
  static void render(OscillatorData &, SingleVoiceData &, double sR);
  Oscillator() = default;
};
} // namespace VeNo::Audio
