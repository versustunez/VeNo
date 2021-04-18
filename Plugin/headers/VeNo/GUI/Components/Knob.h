#pragma once

#include "BaseComponent.h"
#include <JuceHeader.h>

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
namespace VeNo::GUI
{
class Knob : public BaseComponent,
             public juce::Label::Listener,
             public juce::Slider::Listener
{
public:
    Knob (std::string name, std::string showName, size_t id);
    ~Knob();
    // Component
    void paint (juce::Graphics& g) override;
    void resized() override;

    // Juce Label Listener
    void labelTextChanged (juce::Label* labelThatHasChanged) override;
    void editorShown (juce::Label* label, juce::TextEditor& editor) override;
    void editorHidden (juce::Label* label, juce::TextEditor& editor) override;
    void sliderValueChanged (juce::Slider* slider) override;
    void enableLiveLabel();

protected:
    std::unique_ptr<juce::Slider> m_slider;
    std::unique_ptr<SliderAttachment> m_attachment;
    bool m_liveLabel = false;
};
} // namespace VeNo::GUI