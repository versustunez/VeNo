#pragma once

#include "OscillatorStructs.h"

// Oscillator is a Stateless class!
namespace VeNo::Audio {
// not used by LFO because LFO is not the same!
class Oscillator {
public:
  static bool setup(OscillatorData &, size_t instanceId);
  static bool prepare(OscillatorData &);
  static bool process(OscillatorData &, SingleVoiceData &, int currentNote);
  static bool finish(OscillatorData &, SingleVoiceData &);
  static void prepareVoice(OscillatorData &osc, SingleVoiceData voice);
  static float renderVoice(SingleVoiceData &voice, double inc,
                           const Wave &table, int idx);
protected:
  static void updateFrequency(OscillatorData&, SingleVoiceData&, int currentNote);
  static void render(OscillatorData&, SingleVoiceData&);
  Oscillator() = default;

};
} // namespace VeNo::Audio
