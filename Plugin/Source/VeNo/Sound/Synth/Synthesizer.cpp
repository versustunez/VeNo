#include <VeNo/Core/Instance.h>
#include <VeNo/Sound/Generation/Oscillator.h>
#include <VeNo/Sound/Synth/MidiHandler.h>
#include <VeNo/Sound/Synth/Synthesizer.h>
#include <VeNo/Utils/ProfileMacros.h>
#include <random>

namespace VeNo::Audio {
Synthesizer::Synthesizer(size_t instanceID) : m_instanceId(instanceID) {
  m_config = &Core::Config::get();
  m_parameterHandler = Core::Instance::get(instanceID)->handler.get();
  DBGN("Synthesizer Created");
}
void Synthesizer::processBlock(
    juce::AudioBuffer<float> &audioBuffer, juce::MidiBuffer &midiBuffer) {
  VENO_PROFILE_FUNCTION();
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
void Synthesizer::renderVoices(
    juce::AudioBuffer<float> &buffer, int startSample, int numSamples) const {
  VENO_PROFILE_FUNCTION();
  // If VeNo is not playing any note he will skip rendering anything also
  // skipping Matrix because that's how it should be
  if (!hasActiveNote)
    return;

  for (int i = 0; i < numSamples; ++i) {
    // Update Matrix here
    // matrix->update();
    for (int osc = 0; osc < MAX_OSCILLATORS; ++osc) {
      // Update Detune and all here :>
      // Oscillator::update (m_oscillators[osc]);
    }

    // Update Oscillator
    for (const auto &voice : m_voices) {
      // No need to render the Sample for this voice ;)
      if (!voice.isActive || voice.velocity == 0)
        continue;

      // Voice -> Update Frequency
    }

    // Create Rendering  Buffer
    // Do Post-Effect Chain after each voices
  }
}

} // namespace VeNo::Audio
