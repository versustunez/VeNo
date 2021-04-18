// The Parameters can be Generated by the helper/parameter/parameterToCPP.js!
// Any Changes here also Provide to parameter.json
#include <VeNo/Core/Parameter/Handler.h>
#include <VeNo/Core/Parameter/Parameter.h>

namespace VeNo::Core {
void ParameterHandler::setupParameter ()
{
    // Control the Main Volume of the Synth
    addParameterModulate ("master__volume", "Synth Volume", 0, 1.5, 1, Float);
    // Change the Pitch of the Oscillators based on the PitchWheel
    addParameter ("pitch__wheel", "Pitch Wheel", -1, 1, 0, Float);
    // Max Semitones Up
    addParameter ("pitchbend__up", "Pitch Bend Up", 0, 36, 12, Integer);
    // Max Semitones Down
    addParameter ("pitchbend__down", "Pitch Bend Down", 0, 36, 12, Integer);
    // Modulator 1
    addParameter ("mod1", "MOD1", 0, 1, 0, Float);
    // Modulator 2
    addParameter ("mod2", "MOD2", 0, 1, 0, Float);
    // Modulator 3
    addParameter ("mod3", "MOD3", 0, 1, 0, Float);
    // Modulator 4
    addParameter ("mod4", "MOD4", 0, 1, 0, Float);
    // Mono legato -> Disable Voice 2-5 and only enable Voice1 and do Frequency Slides!
    addParameter ("mono_legato", "Mono Legato", 0, 1, 0, Boolean);
    // Glide Time until new note is reached
    addParameter ("portamento", "Portamento", 0, 1500, 0, Float);

    for (int i = 1; i < 5; i++)
    {
        std::string oscId = "osc" + std::to_string (i) + "__";
        std::string OSCName = "OSC" + std::to_string (i);
        // Makes the Oscillator Available for Rendering or not
        addParameter (oscId + "active", OSCName + " Active", 0, 1, i == 1, Boolean);
        // How Many Voices he should play at Once: anyone above 1 will create Unison/Detune Voices
        addParameter (oscId + "voices", OSCName + " Voices", 1, 9, 1, Integer);
        // Shift the Frequency by a specific Semitones amount
        addParameterModulate (oscId + "semitones", OSCName + " Semitones", -24, 24, 0, Integer);
        // Fine tune the Frequency by Cents
        addParameterModulate (oscId + "cents", OSCName + " Fine", -100, 100, 0, Integer);
        // Volume of the Oscillator itself
        addParameterModulate (oscId + "level", OSCName + " Volume", 0, 1, 0.8, Float);
        // Panning of the Oscillator itself
        addParameterModulate (oscId + "panning", OSCName + " Pan", -1, 1, 0, Float);
        // Unison Voice Volume
        addParameterModulate (oscId + "detune_amount", OSCName + " Detune Volume", 0, 1, 0, Float);
        // Unison Voice Spread
        addParameterModulate (oscId + "detune_dense", OSCName + " Detune Strength", 0, 150, 0, Float);
        // Detune Mode
        addParameter (oscId + "detune_mode", OSCName + " Detune Mode", 1, 3, 1, Integer);
        // Shift the Phase of the Oscillator
        addParameterModulate (oscId + "phase", OSCName + " Phase offset", 0, 1, 0, Float);
        // Generate a Random Phase at each new Note
        addParameter (oscId + "random_phase", OSCName + " Random Phase", 0, 1, 0, Boolean);
        // Detune Stereo Spread
        addParameterModulate (oscId + "stereo", OSCName + " Detune Stereo", 0, 200, 50, Float);
        // Main Waveform that is applied without Mix
        addParameter (oscId + "waveform_base", OSCName + " Primary Waveform", 1, 13, 1, Integer);
        // Secondary Waveform that is applied with Mix
        addParameter (oscId + "waveform_sec", OSCName + " Secondary Waveform", 1, 13, 4, Integer);
        // How much should both of the Waveform Mixed
        addParameterModulate (oscId + "waveform_mix", OSCName + " Waveform Mix", 0, 1, 0, Float);
        // Oscillator Envelope Attack
        addParameter (oscId + "attack", OSCName + " Attack", 0, 1500, 10, Float);
        // Oscillator Envelope Decay
        addParameter (oscId + "decay", OSCName + " Decay", 0, 1500, 0, Float);
        // Oscillator Envelope Sustain
        addParameter (oscId + "sustain", OSCName + " Sustain", 0, 1, 1, Float);
        // Oscillator Envelope Release
        addParameter (oscId + "release", OSCName + " Release", 0, 1500, 10, Float);
    }

    for (int i = 1; i < 3; i++)
    {
        std::string lfoId = "lfo" + std::to_string (i) + "__";
        std::string LFOName = "LFO" + std::to_string (i);
        // Active or Deactivate LFO
        addParameter (lfoId + "active", LFOName + " Active", 0, 1, 0, Boolean);
        // How Many Voices he should play at Once: anyone above 1 will create Unison/Detune Voices
        addParameter (lfoId + "voices", LFOName + " Voices", 1, 9, 1, Integer);
        // Frequency of the LFO
        addParameterModulate (lfoId + "rate", LFOName + " Rate", 1, 40, 1, Float);
        // Shift the Phase of the Oscillator
        addParameterModulate (lfoId + "phase", LFOName + " Phase offset", 0, 1, 0, Float);
        // Generate a Random Phase at each new Note
        addParameter (lfoId + "random_phase", LFOName + " Random Phase", 0, 1, 0, Boolean);
        // Main Waveform that is applied without Mix
        addParameter (lfoId + "waveform_base", LFOName + " Primary Waveform", 1, 13, 1, Integer);
        // Secondary Waveform that is applied with Mix
        addParameter (lfoId + "waveform_sec", LFOName + " Secondary Waveform", 1, 13, 4, Integer);
        // How much should both of the Waveform Mixed
        addParameterModulate (lfoId + "waveform_mix", LFOName + " Waveform Mix", 0, 1, 0, Float);
    }

    for (int i = 1; i < 3; i++)
    {
        std::string envId = "env" + std::to_string (i) + "__";
        std::string EnvelopeName = "Envelope" + std::to_string (i);
        // Envelope Attack
        addParameter (envId + "attack", EnvelopeName + " Attack", 0, 1500, 10, Float);
        // Envelope Decay
        addParameter (envId + "decay", EnvelopeName + " Decay", 0, 1500, 0, Float);
        // Envelope Sustain
        addParameter (envId + "sustain", EnvelopeName + " Sustain", 0, 1, 1, Float);
        // Envelope Release
        addParameter (envId + "release", EnvelopeName + " Release", 0, 1500, 10, Float);
    }
    // Select Filter
    addParameter ("dist__filter_one_type", "Distortion Filter One", 1, 5, 1, Integer);
    // Filter Cutoff 20-20000
    addParameterModulate ("dist__filter_one_cutoff", "Distortion Filter One Cutoff", 20, 20000, 20, Float);
    // Select Filter
    addParameter ("dist__filter_two_type", "Distortion Filter Two", 1, 5, 1, Integer);
    // Filter Cutoff 20-20000
    addParameterModulate ("dist__filter_two_cutoff", "Distortion Filter Two Cutoff", 20, 20000, 20, Float);
    // How hard should the Signal Push into it
    addParameter ("dist__input_gain", "Distortion Input Gain", 0, 1.5, 1, Float);
    // How hard should the Signal Go out
    addParameter ("dist__output_gain", "Distortion Output Gain", 0, 1.5, 1, Float);
    // Overdrive it
    addParameter ("dist__drive", "Distortion DRIVE", -16, 16, 0, Float);
    // Distortion Algo selection
    addParameter ("dist__type", "Distortion Type", 1, 7, 1, Integer);
    // Dynamic Parameter that change based on the Algo not all Algo use this
    addParameterModulate ("dist__dynamic", "Distortion Dynamic Parameter", 0, 1, 0, Float);
    // Dry Wet
    addParameterModulate ("dist__mix", "Distortion DRY/WET", 0, 1, 0.5, Float);

}
}