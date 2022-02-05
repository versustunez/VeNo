#pragma once

#include <VeNo/Sound/Generation/EnvelopeStructs.h>
#include <VeNo/Sound/Generation/OscillatorStructs.h>

namespace VeNo::Audio {
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
  static void noteOn(
      SynthVoice &voice, int channel, int note, float velocity, bool legato);
  static void noteOff(SynthVoice &voice, float velocity);
  static void clear(SynthVoice &voice);
};
} // namespace VeNo::Audio