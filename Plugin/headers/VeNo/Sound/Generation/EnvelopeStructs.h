#pragma once

namespace VeNo::Audio {
enum EnvelopeState { IDLE = 0, ATTACK, DECAY, SUSTAIN, RELEASE };
struct RawEnvelope {
  double attack;
  double decay;
  double release;
};
struct EnvelopeData {
  double sampleRate;
  double attack;
  double decay;
  double sustain;
  double release;
  RawEnvelope raw;
};
struct EnvelopeVoiceData {
  EnvelopeState state;
  double value;
};
} // namespace VeNo::Audio