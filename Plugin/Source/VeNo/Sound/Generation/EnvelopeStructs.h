#pragma once

#include <VeNo/Core/Parameter/Parameter.h>

namespace VeNo::Audio {
enum EnvelopeState { IDLE = 0, ATTACK, DECAY, SUSTAIN, RELEASE };
struct RawEnvelope {
  Core::Parameter *attack{nullptr};
  Core::Parameter *decay{nullptr};
  Core::Parameter *release{nullptr};
  Core::Parameter *sustain{nullptr};
};
struct EnvelopeData {
  double sampleRate{44100};
  double attack{0.01};
  double decay{0.1};
  double sustain{1.0};
  double release{0.01};
  bool needRecalculate{true};
  RawEnvelope parameters{};
};
struct EnvelopeVoiceData {
  EnvelopeState state{};
  double value{0.0};
  double releaseRate{0.0};
};
} // namespace VeNo::Audio