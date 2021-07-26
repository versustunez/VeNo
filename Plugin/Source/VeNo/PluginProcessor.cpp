#include <VeNo/Core/Config.h>
#include <VeNo/PluginEditor.h>
#include <VeNo/PluginProcessor.h>
#include <VeNo/Sound/Generation/WaveTables/Creator/Generator.h>

VeNoProcessor::VeNoProcessor()
    : AudioProcessor (BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      instance (VeNo::Core::Instance::create()),
      m_id (juce::Uuid().toString().toStdString()),
      treeState (*this, nullptr, "VeNo", instance->handler->setupProcessor())
{
    instance->treeState = &treeState;
    VeNo::Core::Config::get().registerProcessor (m_id, this);
}

void VeNoProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                  juce::MidiBuffer& /*midiMessages*/)
{
    // oversampling only if less then 96khz everything above should fine by itself and "too hungry"
}

juce::AudioProcessorEditor* VeNoProcessor::createEditor()
{
    return new VeNoEditor (*this, m_id);
}

void VeNoProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void VeNoProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}
VeNoProcessor::~VeNoProcessor()
{
    VeNo::Core::Config::get().removeProcessor (m_id);
    VeNo::Core::Instance::remove (instance->id);
}
void VeNoProcessor::prepareToPlay (double sampleRate, int blockSize)
{
    auto& config = VeNo::Core::Config::get();
    if (config.sampleRate != sampleRate)
    {
        config.sampleRate = sampleRate;
        // VeNo::Audio::Waves::WaveTableGenerator::getInstance().init();
        // VeNo::Audio::Waves::WaveTableGenerator will later be setup to generate user selected waves!
    }
    //Setup WaveTables on Change
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VeNoProcessor();
}
