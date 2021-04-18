#pragma once

#include <JuceHeader.h>
#include <VeNo/PluginEditor.h>
#include <VeNo/PluginProcessor.h>
#include <string>
#include <VeNo/GUI/GuiLang/GUIHandler.h>

class VeNoEditor : public juce::AudioProcessorEditor
{
public:
    explicit VeNoEditor (VeNoProcessor& p, std::string id);
    ~VeNoEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

protected:
    std::string m_id;
    VeNo::GUI::Interpreter mainInterpreter;
};
