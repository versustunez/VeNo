#pragma once

#include <VeNo/Sound/Generation/EnvelopeStructs.h>
#include <VeNo/Sound/Generation/OscillatorStructs.h>

namespace VeNo::Audio {
class Synthesizer;
struct SynthVoice {
  VoiceData voiceData{};
  EnvelopeVoiceData envelopeData{};
  int currentNote{-1};
  int currentChannel{-1};
  bool isActive{false};
  float velocity{0.0};
  uint64_t noteOnTime{0};
  bool legato{false};
};

struct SynthVoiceHelper {
  static void noteOn(Synthesizer &synth, SynthVoice &voice, int channel,
                     int note, float velocity, bool legato);
  static void noteOff(Synthesizer &synth, SynthVoice &voice, float velocity);
  static void clear(Synthesizer &synth, SynthVoice &voice);
};
} // namespace VeNo::Audio