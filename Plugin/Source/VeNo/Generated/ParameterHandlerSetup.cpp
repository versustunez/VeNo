// The Parameters can be Generated by the helper/parameter/parameterToCPP.js!
// Any Changes here also Provide to parameter.json
#include <VeNo/Core/Parameter/Handler.h>
#include <VeNo/Core/Parameter/Parameter.h>

namespace VeNo::Core {
void ParameterHandler::setupParameter () {
  addParameterModulate ("master__volume", "Synth Volume", 0, 1.5, 1, Float);
  addParameter ("after__touch", "After Touch", 0, 1, 0, Float);
  addParameter ("mod__wheel", "Mod Wheel", 0, 1, 0, Float);
  addParameter ("pitch__wheel", "Pitch Wheel", -1, 1, 0, Float);
  addParameter ("pitchbend__up", "Pitch Bend Up", 0, 36, 12, Integer);
  addParameter ("pitchbend__down", "Pitch Bend Down", 0, 36, 12, Integer);
  addParameter ("mod1", "MOD1", 0, 1, 0, Float);
  addParameter ("mod2", "MOD2", 0, 1, 0, Float);
  addParameter ("mod3", "MOD3", 0, 1, 0, Float);
  addParameter ("mod4", "MOD4", 0, 1, 0, Float);
  addParameter ("mono_legato", "Mono Legato", 0, 1, 0, Boolean);
  addParameter ("portamento", "Portamento", 0, 1500, 0, Float);

  for (int i = 1; i < 5; i++) {
    std::string oscId = "osc" + std::to_string (i) + "__";
    std::string OSCName = "OSC" + std::to_string (i);
    addParameter (oscId + "active", OSCName + " Active", 0, 1, i == 1, Boolean);
    addParameter (oscId + "voices", OSCName + " Voices", 1, 9, 1, Integer);
    addParameterModulate (oscId + "semitones", OSCName + " Semitones", -24, 24, 0, Integer);
    addParameterModulate (oscId + "cents", OSCName + " Fine", -100, 100, 0, Integer);
    addParameterModulate (oscId + "level", OSCName + " Volume", 0, 1, 0.8, Float);
    addParameterModulate (oscId + "panning", OSCName + " Pan", -1, 1, 0, Float);
    addParameterModulate (oscId + "detune_amount", OSCName + " Detune Volume", 0, 1, 0, Float);
    addParameterModulate (oscId + "detune_dense", OSCName + " Detune Strength", 0, 150, 0, Float);
    addParameter (oscId + "detune_mode", OSCName + " Detune Mode", 1, 4, 2, Integer);
    addParameterModulate (oscId + "phase", OSCName + " Phase offset", 0, 1, 0, Float);
    addParameter (oscId + "random_phase", OSCName + " Random Phase", 0, 1, 0, Boolean);
    addParameterModulate (oscId + "stereo", OSCName + " Detune Stereo", 0, 200, 100, Float);
    addParameter (oscId + "wave", OSCName + " Waveform", 1, 4, 1, Integer);
  }

  for (int i = 1; i < 3; i++) {
    std::string lfoId = "lfo" + std::to_string (i) + "__";
    std::string LFOName = "LFO" + std::to_string (i);
    addParameter (lfoId + "active", LFOName + " Active", 0, 1, 0, Boolean);
    addParameterModulate (lfoId + "rate", LFOName + " Rate", 1, 40, 1, Float);
    addParameter (lfoId + "wave", LFOName + " Waveform", 1, 4, 1, Integer);
  }

  for (int i = 1; i < 3; i++) {
    std::string rngId = "rng" + std::to_string (i) + "__";
    std::string RandomName = "Random" + std::to_string (i);
    addParameter (rngId + "active", RandomName + " Active", 0, 1, 0, Boolean);
    addParameterModulate (rngId + "change_rate", RandomName + " Change Rate", 16, 4096, 256, Integer);
    addParameterModulate (rngId + "mode", RandomName + " Mode", 1, 2, 1, Integer);
  }

  for (int i = 1; i < 5; i++) {
    std::string envId = "env" + std::to_string (i) + "__";
    std::string EnvelopeName = "Envelope" + std::to_string (i);
    addParameter (envId + "attack", EnvelopeName + " Attack", 0, 1500, 10, Float);
    addParameter (envId + "decay", EnvelopeName + " Decay", 0, 1500, 0, Float);
    addParameter (envId + "sustain", EnvelopeName + " Sustain", 0, 1, 1, Float);
    addParameter (envId + "release", EnvelopeName + " Release", 0, 1500, 10, Float);
  }

  addParameter ("dist__filter_one_type", "Distortion Filter One", 1, 5, 1, Integer);
  addParameterModulate ("dist__filter_one_cutoff", "Distortion Filter One Cutoff", 20, 20000, 20, Float);
  addParameter ("dist__filter_two_type", "Distortion Filter Two", 1, 5, 1, Integer);
  addParameterModulate ("dist__filter_two_cutoff", "Distortion Filter Two Cutoff", 20, 20000, 20, Float);
  addParameter ("dist__input_gain", "Distortion Input Gain", 0, 1.5, 1, Float);
  addParameter ("dist__output_gain", "Distortion Output Gain", 0, 1.5, 1, Float);
  addParameter ("dist__drive", "Distortion DRIVE", -16, 16, 0, Float);
  addParameter ("dist__type", "Distortion Type", 1, 7, 1, Integer);
  addParameterModulate ("dist__dynamic", "Distortion Dynamic Parameter", 0, 1, 0, Float);
  addParameterModulate ("dist__mix", "Distortion DRY/WET", 0, 1, 0.5, Float);

}
} // namespace VeNo::Core