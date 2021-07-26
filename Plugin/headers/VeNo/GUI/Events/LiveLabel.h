#pragma once
#include <JuceHeader.h>
#include <VeNo/GUI/Components/Knob.h>

namespace VeNo::GUIEvents {

class LiveLabel : public juce::Slider::Listener,
                  juce::Timer
{
public:
    LiveLabel(VeNo::GUI::Knob* knob, bool isSemi, std::string showName);
    ~LiveLabel() override;
    void stopLive();
    void sliderValueChanged (juce::Slider* slider) override;
    void sliderDragStarted (juce::Slider* slider) override;
    void sliderDragEnded (juce::Slider* slider) override;

private:
    void timerCallback() override;

protected:
    VeNo::GUI::Knob* m_knob;
    bool timerStarted;
    bool semiMode = false;
    int ticks;
    std::string m_showName;
    bool m_sliderDragging = false;
};
}