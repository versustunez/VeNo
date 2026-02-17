#pragma once

#include "Matrix.h"
#include "ParameterCacheStruct.h"
#include "ParameterEventHandler.h"
#include "SynthVoice.h"
#include "VeNo/Sound/DSP/FX/Filter.h"

#include <JuceHeader.h>
#include <VeNo/Core/Config.h>
#include <VeNo/Core/Parameter/Handler.h>
#include <VeNo/Sound/DSP/FXChain.h>
#include <VeNo/Sound/Generation/OscillatorStructs.h>

#define MAX_VOICES 5

namespace VeNo::Audio {
class MidiHandler;
class Synthesizer {
public:
  explicit Synthesizer(size_t instanceID);
  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &);
  [[nodiscard]] size_t instanceID() const { return m_instanceId; }
  [[nodiscard]] Core::ParameterHandler *parameterHandler() const {
    return m_parameterHandler;
  }
  SynthVoice *voices() { return m_voices; }
  void setSampleRate(double sampleRate);

  void invalidateEnvelopes();

  EnvelopeData &envelope();
  Matrix &matrix() { return m_matrix; }
  OptimizedOscillatorState &oscillatorState(int idx) {
    return m_oscillators[idx].state;
  }

private:
  void renderVoices(juce::AudioBuffer<float> &, int startSample,
                    int numSamples);
  void addEvents();

public:
  CachedCoreParameters ParameterCache{};

private:
  friend MidiHandler;
  InstanceID m_instanceId{0};
  Core::ParameterHandler *m_parameterHandler{nullptr};
  Core::Config *m_config{nullptr};
  SynthVoice m_voices[MAX_VOICES]{};
  FXChain *m_FXChain{nullptr};
  ParameterEventHandler m_parameterEventHandler{};
  uint64_t lastNoteOnCounter{0};
  bool hasActiveNote{false};
  OscillatorData m_oscillators[OSCILLATORS];
  EnvelopeData m_envelope;
  juce::CriticalSection lock;
  Matrix m_matrix;
  double m_sampleRate{44100};
  OscillatorModulatorState m_OscillatorState[OSCILLATORS]{};

private:
  friend ParameterEventHandler;
};
} // namespace VeNo::Audio
