#include <VeNo/Core/Instance.h>
#include <VeNo/Definitions.h>
#include <VeNo/Sound/Generation/Envelope.h>
#include <VeNo/Sound/Generation/Oscillator.h>
#include <VeNo/Sound/Synth/MidiHandler.h>
#include <VeNo/Sound/Synth/Synthesizer.h>
#include <random>

namespace VeNo::Audio {
Synthesizer::Synthesizer(size_t instanceID)
    : m_instanceId(instanceID),
      m_matrix(instanceID) {
  m_config = &Core::Config::get();
  auto *instance = Core::Instance::get(instanceID);
  m_parameterHandler = instance->handler.get();
  instance->state.RegisterSynth();
  m_FXChain = instance->state.FXChain.get();
  m_FXChain->SetSampleRate(m_sampleRate);
  ParameterCache.setup(m_parameterHandler);
  m_envelope = CreateRef<EnvelopeData>();
  Envelope::setup(*m_envelope, m_instanceId, "env1");
  double val = ParameterCache.Portamento->getValue() / 1000.0;
  for (auto &m_voice : m_voices) {
    m_voice = CreateScope<SynthVoice>();
    m_voice->midiNotePortamento.reset(m_sampleRate, val);
  }
  for (int i = 0; i < OSCILLATORS; ++i) {
    m_oscillators[i] = CreateRef<OscillatorData>();
    m_oscillators[i]->id = i + 1;
    Oscillator::setup(*m_oscillators[i], m_instanceId);
  }
  addEvents();
  DBGN("Synthesizer Created");
}
void Synthesizer::processBlock(juce::AudioBuffer<float> &audioBuffer,
                               juce::MidiBuffer &midiBuffer) {
  int startSample = 0;
  int numSamples = audioBuffer.getNumSamples();
  const juce::ScopedLock sl(lock);
  const int targetChannels = audioBuffer.getNumChannels();
  auto midiIterator = midiBuffer.findNextSamplePosition(startSample);
  if (targetChannels == 0)
    return;
  juce::MidiMessage midi;
  bool firstEvent = true;
  for (; numSamples > 0; ++midiIterator) {
    if (midiIterator == midiBuffer.cend())
      return renderVoices(audioBuffer, startSample, numSamples);
    const auto metadata = *midiIterator;
    const int samplesToNextMidiMessage = metadata.samplePosition - startSample;
    if (samplesToNextMidiMessage >= numSamples) {
      renderVoices(audioBuffer, startSample, numSamples);
      MidiHandler::handleMidiMessage(metadata.getMessage(), *this);
      break;
    }

    if (samplesToNextMidiMessage < (firstEvent ? 1 : 8)) {
      MidiHandler::handleMidiMessage(metadata.getMessage(), *this);
      continue;
    }

    firstEvent = false;
    renderVoices(audioBuffer, startSample, samplesToNextMidiMessage);
    MidiHandler::handleMidiMessage(metadata.getMessage(), *this);
    startSample += samplesToNextMidiMessage;
    numSamples -= samplesToNextMidiMessage;
  }
  std::for_each(midiIterator, midiBuffer.cend(),
                [&](const juce::MidiMessageMetadata &meta) {
                  MidiHandler::handleMidiMessage(meta.getMessage(), *this);
                });
}
void Synthesizer::setSampleRate(double sampleRate) {
  assert(sampleRate > 0);
  m_sampleRate = sampleRate;
  m_envelope->sampleRate = sampleRate;
  m_envelope->needRecalculate = true;
  m_FXChain->SetSampleRate(sampleRate);
  m_matrix.handle().setSampleRate(sampleRate);
  m_Filter.update();
}
void Synthesizer::renderVoices(juce::AudioBuffer<float> &buffer,
                               int startSample, int numSamples) {
  // If VeNo is not playing any note he will skip rendering anything also
  // skipping Matrix because that's how it should be
  if (!hasActiveNote)
    return;

  for (int i = 0; i < numSamples; ++i) {
    Channel outChannel{};
    // Update Matrix here
    m_matrix.update();
    if (Envelope::prepare(*m_envelope)) {
      for (const auto &voice : m_voices)
        Envelope::needNextStep(voice->envelopeData, *m_envelope);
    }
    for (const auto &oscillator : m_oscillators) {
      Oscillator::prepare(*oscillator);
    }

    for (auto &voice : m_voices) {
      if (!voice->isActive || voice->velocity == 0)
        continue;
      double envelope = Envelope::process(voice->envelopeData, *m_envelope);
      if (envelope == 0 || voice->envelopeData.state == EnvelopeState::IDLE) {
        // clear Voice
        SynthVoiceHelper::clear(*this, *voice.get());
        continue;
      }
      if (voice->isDirty) {
        for (int j = 0; j < OSCILLATORS; ++j) {
          Oscillator::prepareVoice(*m_oscillators[j],
                                   voice->voiceData.oscillatorVoices[j]);
        }
        voice->isDirty = false;
      }

      Channel voiceData{};
      for (int j = 0; j < OSCILLATORS; ++j) {
        auto &voiceD = voice->voiceData.oscillatorVoices[j];
        if (Oscillator::process(*m_oscillators[j], voiceD,
                                voice->midiNotePortamento.getNextValue(),
                                m_sampleRate)) {
          voiceData.left += voiceD.output.left;
          voiceData.right += voiceD.output.right;
        }
      }
      outChannel.left += voiceData.left * envelope * 0.25;
      outChannel.right += voiceData.right * envelope * 0.25;
    }

    // Create Rendering  Buffer
    // Do Post-Effect Chain after each voices

    outChannel.left *= ParameterCache.MasterVolume->getValue();
    outChannel.right *= ParameterCache.MasterVolume->getValue();
    m_FXChain->process(outChannel);
    m_Filter.process(outChannel);
    buffer.addSample(0, startSample, (float)outChannel.left);
    buffer.addSample(1, startSample, (float)outChannel.right);
    ++startSample;
  }
}
void Synthesizer::addEvents() {
  m_parameterEventHandler.setSynthesizer(this);
  auto &handler = Core::Instance::get(m_instanceId)->eventHandler;
  handler.addHandler("env1__attack", &m_parameterEventHandler);
  handler.addHandler("env1__decay", &m_parameterEventHandler);
  handler.addHandler("env1__release", &m_parameterEventHandler);
  handler.addHandler("env1__sustain", &m_parameterEventHandler);
  handler.addHandler("portamento", &m_parameterEventHandler);
}
void Synthesizer::invalidateEnvelopes() { m_envelope->needRecalculate = true; }
EnvelopeData &Synthesizer::envelope() { return *m_envelope; }

} // namespace VeNo::Audio
