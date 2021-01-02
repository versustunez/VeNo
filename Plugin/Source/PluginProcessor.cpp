#include "PluginProcessor.h"
#include "PluginEditor.h"


VeNoProcessor::VeNoProcessor()
{
}

void VeNoProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                 juce::MidiBuffer& /*midiMessages*/)

{
}

juce::AudioProcessorEditor* VeNoProcessor::createEditor()
{
    return new VeNoEditor(*this);
}

void VeNoProcessor::getStateInformation(juce::MemoryBlock& destData)
{
}

void VeNoProcessor::setStateInformation(const void* data, int sizeInBytes)
{
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VeNoProcessor();
}
