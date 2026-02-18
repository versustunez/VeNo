#include "Handler.h"
#include "Parameter.h"

#include <VUtils/LogInclude.h>
#include <VeNo/Sound/DSP/FX/FXEnums.h>
#include <VeNo/Sound/DSP/FX/Impl/Filter.h>

// THIS FILE FOLLOWS A SPECIFIC: ONE PARAMETER PER LINE!

namespace VeNo::Core {
// clang-format off
void ParameterHandler::setupParameter () {
  const auto addFilter = [&](const std::string& id, const std::string& name, bool hasQ, bool useGain, bool isLimited = false) {
    addParameterModulate(id + "frequency", name + " Frequency", 20, 20000, 20000, Float);
    addParameter (id + "type", name + " Type", 1, static_cast<size_t>(FilterImpl::Type::END), 1, Integer);
    if (hasQ)
      addParameterModulate (id + "q_factor", name + " QFactor", 0.2, 6, 0.7, Float);
    if (useGain)
      addParameterModulate (id + "gain", name + " Gain", -6.0, 6.0, 0.0, Float);
  };

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
    std::string id = fmt::format("osc{}__", i);
    std::string name = fmt::format("OSC {}", i);
    addParameter (id + "active", name + " Active", 0, 1, i == 1, Boolean);
    addParameter (id + "voices", name + " Voices", 1, 9, 1, Integer);
    addParameterModulate (id + "semitones", name + " Semitones", -24, 24, 0, Integer);
    addParameterModulate (id + "cents", name + " Fine", -100, 100, 0, Integer);
    addParameterModulate (id + "level", name + " Volume", 0, 1, 0.8, Float);
    addParameterModulate (id + "panning", name + " Pan", -1, 1, 0, Float);
    addParameterModulate (id + "detune_amount", name + " Detune Volume", 0, 1, 0, Float);
    addParameterModulate (id + "detune_dense", name + " Detune Strength", 0, 150, 0, Float);
    addParameter (id + "detune_mode", name + " Detune Mode", 1, 4, 2, Integer);
    addParameterModulate (id + "phase", name + " Phase offset", 0, 1, 0, Float);
    addParameter (id + "random_phase", name + " Random Phase", 0, 1, 0, Boolean);
    addParameter (id + "retrigger", name + " Retrigger", 0, 1, 1, Boolean);
    addParameterModulate (id + "stereo", name + " Detune Stereo", 0, 200, 100, Float);
    addParameter (id + "wave", name + " Waveform", 1, 4, 1, Integer);
  }

  for (int i = 1; i < 3; i++) {
    std::string id = fmt::format("lfo{}__", i);
    std::string name = fmt::format("LFO {}", i);
    addParameter (id + "active", name + " Active", 0, 1, 0, Boolean);
    addParameterModulate (id + "rate", name + " Rate", 1, 40, 1, Float);
    addParameter (id + "wave", name + " Waveform", 1, 4, 1, Integer);
  }

  for (int i = 1; i < 3; i++) {
    std::string id = fmt::format("rng{}__", i);
    std::string name = fmt::format("Random {}", i);
    addParameter (id + "active", name + " Active", 0, 1, 0, Boolean);
    addParameterModulate (id + "change_rate", name + " Change Rate", 8, 4096, 256, Integer);
    addParameter(id + "mode", name + " Mode", 1, 3, 1, Integer);
  }

  for (int i = 1; i < 5; i++) {
    std::string id = fmt::format("env{}__", i);
    std::string name = fmt::format("Envelope {}", i);
    addParameter (id + "attack", name + " Attack", 0, 1500, 10, Float);
    addParameter (id + "decay", name + " Decay", 0, 1500, 0, Float);
    addParameter (id + "sustain", name + " Sustain", 0, 1, 1, Float);
    addParameter (id + "release", name + " Release", 0, 1500, 10, Float);
  }

  {
    addFilter("dist__filter__", "Distortion Filter", true, false, true);
    addParameter ("dist__input_gain", "Distortion Input Gain", 0, 3, 1, Float);
    addParameter ("dist__output_gain", "Distortion Output Gain", 0, 3, 1, Float);
    addParameter ("dist__type", "Distortion Type", 1, static_cast<int>(Audio::DistortionMode::END), 1, Integer);
    addParameterModulate("dist__drive", "Distortion DRIVE", 0, 32, 0, Float);
    addParameterModulate ("dist__dynamic1", "Distortion Dynamic Parameter", 0, 1, 0, Float);
    addParameterModulate ("dist__mix", "Distortion DRY/WET", 0, 1, 0.5, Float);
  }

  for (int i = 1; i < 3; i++) {
    std::string id = fmt::format("filter{}__", i);
    std::string name = fmt::format("Filter {}", i);
    addFilter(id, name, true, true);
  }

  {
    addParameter("compressor_threshold", "Compressor Threshold", -70, 0, 0, Float);
    addParameter("compressor_ratio", "Compressor Ratio", 1, 10, 1, Float);
    addParameter("compressor_attack", "Compressor Attack", 1, 1000, 1, Float);
    addParameter("compressor_release", "Compressor Release", 1, 1000, 1, Float);
    addParameter("compressor_active", "Compressor Active", 0, 1, 0, Boolean);
  }
}
} // namespace VeNo::Core
// clang-format on
