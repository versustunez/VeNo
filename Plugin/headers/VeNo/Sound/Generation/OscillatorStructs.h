#pragma once
#include "../Structs.h"
#include <VeNo/Core/Parameter/Parameter.h>
#include <cstdint>

namespace VeNo::Audio {
// holds the Pointers to Parameters for easy Access
struct OscillatorState {
  typedef Core::Parameter Parameter;
  Parameter *active;
  Parameter *voices;
  Parameter *semitones;
  Parameter *cents;
  Parameter *level;
  Parameter *panning;
  Parameter *detuneAmount;
  Parameter *detuneDense;
  Parameter *detuneMode;
  Parameter *phase;
  Parameter *randomPhase;
  Parameter *stereo;
  Parameter *wavePosition;
  Parameter *attack;
  Parameter *decay;
  Parameter *sustain;
  Parameter *release;
};

struct OscillatorData {
  uint8_t id = 0;
  double *wave = nullptr;
  OscillatorState state;
};

// holds a state to phase offset and start
struct UnisonVoice {
  float phaseOffset{0};
  float phaseInc{0};
};

struct SingleVoiceData {
  uint8_t id = 0;
  uint8_t voices = 1; // min voices 1 :) 0 voices = deactivated
  UnisonVoice unisonVoices[MAX_UNISON_VOICES]{};
  Channel outData[MAX_UNISON_VOICES]{};
};

struct VoiceData {
  SingleVoiceData oscillatorVoices[MAX_OSCILLATORS]{};
};
} // namespace VeNo::Audio
