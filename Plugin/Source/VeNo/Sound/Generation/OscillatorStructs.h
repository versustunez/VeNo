#pragma once
#include "../Structs.h"

#include <VeNo/Core/Parameter/Parameter.h>
#include <VeNo/Sound/DSP/Detune/DetuneStruct.h>
#include <VeNo/Sound/DSP/Widener.h>
#include <cstdint>

namespace VeNo::Audio {
// holds the Pointers to Parameters for easy Access
struct OscillatorState {
  typedef Core::Parameter Parameter;
  typedef Core::ModulateParameter ModParameter;
  Parameter *active;
  Parameter *voices;
  ModParameter *semitones;
  ModParameter *cents;
  ModParameter *level;
  ModParameter *panning;
  ModParameter *detuneAmount;
  ModParameter *detuneDense;
  Parameter *detuneMode;
  ModParameter *phase;
  Parameter *randomPhase;
  Parameter *retrigger;
  ModParameter *stereo;
  Parameter *wave;
  Parameter *pitchWheel;
  Parameter *pitchBendUp;
  Parameter *pitchBendDown;
  WaveLib *lib;
};

struct OscillatorData {
  int id{0};
  WaveTable* group{};
  OscillatorState state{};
  DetuneState detuneState;
  WidenerState widenerState;
};

// holds a state to phase offset and start
struct UnisonVoice {
  float phaseOffset{0};
  float phaseInc{0};
};

struct SingleVoiceData {
  double frequency{};
  int id = 0;
  UnisonVoice unisonVoices[MAX_UNISON_VOICES]{};
  Channel outData[MAX_UNISON_VOICES]{};
  Channel output{};
};

struct VoiceData {
  SingleVoiceData oscillatorVoices[OSCILLATORS]{};
};
} // namespace VeNo::Audio
