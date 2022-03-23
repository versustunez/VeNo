#include <VUtils/Logging.h>
#include <VeNo/Core/Config.h>
#include <VeNo/PluginEditor.h>
#include <VeNo/PluginProcessor.h>
#include <VeNo/Utils/ProfileMacros.h>

VeNoProcessor::VeNoProcessor()
    : AudioProcessor(BusesProperties().withOutput(
          "Output", juce::AudioChannelSet::stereo(), true)),
      instance(VeNo::Core::Instance::create()),
      m_id(juce::Uuid().toString().toStdString()),
      treeState(*this, nullptr, "VeNo", instance->handler->setupProcessor()) {
  instance->treeState = &treeState;
  VeNo::Core::Config::get().registerProcessor(m_id, this);
  VENO_PROFILE_BEGIN_SESSION("VeNo", "res://PROFILES/" + m_id);
}

void VeNoProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                 juce::MidiBuffer &midiMessages) {
  VENO_PROFILE_FUNCTION();
  auto *head = getPlayHead();
  if (head) {
    juce::AudioPlayHead::CurrentPositionInfo currentPositionInfo;
    getPlayHead()->getCurrentPosition(currentPositionInfo);
  }
  buffer.clear();
  // oversampling only if less than 96khz everything above should fine by itself
  // and "too hungry"

  if (instance->synthesizer)
    instance->synthesizer->processBlock(buffer, midiMessages);
}

juce::AudioProcessorEditor *VeNoProcessor::createEditor() {
  return new VeNoEditor(*this, m_id);
}

void VeNoProcessor::getStateInformation(juce::MemoryBlock &) {
  VENO_PROFILE_FUNCTION();
}

void VeNoProcessor::setStateInformation(const void *, int) {
  VENO_PROFILE_FUNCTION();
}
VeNoProcessor::~VeNoProcessor() {
  VeNo::Core::Config::get().removeProcessor(m_id);
  VeNo::Core::Instance::remove(instance->id);
  VENO_PROFILE_END_SESSION();
}
void VeNoProcessor::prepareToPlay(double sampleRate, int /*blockSize*/) {
  VENO_PROFILE_FUNCTION();
  auto &config = VeNo::Core::Config::get();
  if (config.sampleRate != sampleRate) {
    config.sampleRate = sampleRate;
  }
  if (!instance->synthesizer)
    instance->synthesizer =
        VeNo::CreateRef<VeNo::Audio::Synthesizer>(instance->id);
  instance->synthesizer->setSampleRate(sampleRate);
  // YES SAMPLE RATE CHANGED REGENERATE ALL OF THEM ;)
  for (auto &generator : instance->waveHolder.generators) {
    generator->regenerateAll();
  }
}

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new VeNoProcessor();
}
