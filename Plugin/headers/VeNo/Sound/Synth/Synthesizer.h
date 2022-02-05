#pragma once

#include "SynthVoice.h"
#include "VeNo/Sound/Generation/OscillatorStructs.h"
#include <JuceHeader.h>
#include <VeNo/Core/Config.h>
#include <VeNo/Core/Parameter/Handler.h>

#define MAX_VOICES 5

namespace VeNo::Audio {
class MidiHandler;
class Synthesizer {
public:
  explicit Synthesizer(size_t instanceID);
  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &);
  size_t instanceID() const { return m_instanceId; };
  Core::ParameterHandler *parameterHandler() { return m_parameterHandler; };
  SynthVoice *voices() { return m_voices; };

private:
  friend MidiHandler;
  void renderVoices(
      juce::AudioBuffer<float> &, int startSample, int numSamples) const;
  Core::Config *m_config{nullptr};
  size_t m_instanceId{0};
  SynthVoice m_voices[MAX_VOICES]{};
  Core::ParameterHandler *m_parameterHandler{nullptr};
  uint64_t lastNoteOnCounter{0};
  bool hasActiveNote{false};
  OscillatorData m_oscillators[MAX_OSCILLATORS]{};
  juce::CriticalSection lock;
};
} // namespace VeNo::Audio
