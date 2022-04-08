#pragma once
#include <VeNo/Definitions.h>
namespace VeNo::Audio {
struct DetuneChannel {
  float left, right;
};
struct DetuneState {
  float lookup[MAX_UNISON_VOICES]{};
  DetuneChannel volume[MAX_UNISON_DETUNE]{};
  int lastVoices{0};
  float lastDetune{0};
  int midiNote{0};
  int lastMode{0};
};
} // namespace VeNo::Audio
