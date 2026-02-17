#pragma once
#include "../Structs.h"

#include <VeNo/Core/Parameter/Parameter.h>
#include <VeNo/Sound/DSP/Detune/DetuneStruct.h>
#include <VeNo/Sound/DSP/Widener.h>
#include <cstdint>

namespace VeNo::Audio {
// holds the Pointers to Parameters for easy Access
struct OscillatorModulatorState {
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

struct OptimizedOscillatorState {
  bool active{false};
  bool randomPhase{true};
  bool retrigger{true};
  uint8_t voices{1};
  uint8_t detuneMode{0};
  uint8_t wave{0};
  float pitchWheel{};
  float pitchBendUp{};
  float pitchBendDown{};
  float detuneDense[MAX_VOICES]{};
  float semitones[MAX_VOICES]{};
  float cents[MAX_VOICES]{};
  float level[MAX_VOICES]{};
  float panning[MAX_VOICES]{};
  float detuneAmount[MAX_VOICES]{};
  float phase[MAX_VOICES]{};
  float stereo[MAX_VOICES]{};
  WaveLib *lib{};
};

struct OscillatorData {
  int id{0};
  WaveTable *group{};
  OptimizedOscillatorState state{};
  DetuneState detuneState[MAX_VOICES];
  WidenerState widenerState[MAX_VOICES];
};

struct SingleVoiceData {
  int id = 0;
  double frequency{};
  Channel output{};
  float phaseOffset[MAX_UNISON_VOICES]{0};
  float phaseInc[MAX_UNISON_VOICES]{0};
  Channel outData[MAX_UNISON_VOICES]{};
};

struct VoiceData {
  SingleVoiceData oscillatorVoices[OSCILLATORS]{};
};
} // namespace VeNo::Audio
