#pragma once

#include "JuceHeader.h"
#include "PluginProcessor.h"

class VeNoEditor : public juce::AudioProcessorEditor
{
public:
    explicit VeNoEditor(VeNoProcessor&);

    void paint(juce::Graphics&) override;
    void resized() override {}
};
