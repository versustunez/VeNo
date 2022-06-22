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
    auto position = head->getPosition();
    if (position.hasValue() && position->getBpm().hasValue()) {
      instance->bpm = *position->getBpm();
    }
  }
  buffer.clear();
  if (instance->synthesizer) {
    instance->synthesizer->processBlock(buffer, midiMessages);
    instance->buffer->addSamples(buffer);
  }
}

juce::AudioProcessorEditor *VeNoProcessor::createEditor() {
  return new VeNoEditor(*this, m_id);
}

// @TODO: Important needed
void VeNoProcessor::getStateInformation(juce::MemoryBlock &destData) {
  VENO_PROFILE_FUNCTION();
  copyXmlToBinary (*instance->state.PresetManager->GetCurrentData (), destData);
}

// @TODO: IMPORTANT needed;)
void VeNoProcessor::setStateInformation(const void * data, int sizeInBytes) {
  VENO_PROFILE_FUNCTION();
  VeNo::Scope<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
  if (xmlState != nullptr)
    instance->state.PresetManager->SetCurrentData(xmlState);
}

VeNoProcessor::~VeNoProcessor() {
  VeNo::Core::Config::get().removeProcessor(m_id);
  VeNo::Core::Instance::remove(instance->id);
  VENO_PROFILE_END_SESSION();
}
void VeNoProcessor::prepareToPlay(double sampleRate, int _blockSize) {
  VENO_PROFILE_FUNCTION();
  auto &config = VeNo::Core::Config::get();
  if (config.sampleRate != sampleRate) {
    config.sampleRate = sampleRate;
  }
  VeNo::Audio::WaveLib::Get().CreateAll();
  if (!instance->synthesizer)
    instance->synthesizer =
        VeNo::CreateRef<VeNo::Audio::Synthesizer>(instance->id);
  instance->synthesizer->setSampleRate(sampleRate);
  instance->buffer->reset(_blockSize);
  instance->eventHandler.triggerEvent("audioLibCreated", new VeNo::Events::AddEvent);
}

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new VeNoProcessor();
}
