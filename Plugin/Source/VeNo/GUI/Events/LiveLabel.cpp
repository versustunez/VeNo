#include <VeNo/GUI/Events/LiveLabel.h>

namespace VeNo::GUIEvents
{
LiveLabel::LiveLabel (VeNo::GUI::Knob* knob, bool isSemi, std::string showName)
    : m_knob (knob), semiMode (isSemi), m_showName (std::move (showName))
{
    m_knob->slider()->addListener (this);
}
void LiveLabel::timerCallback()
{
    if (m_sliderDragging)
        return;
    ticks++;
    if (ticks < 5)
        return;
    m_knob->setText (m_knob->showName());
    stopTimer();
    timerStarted = false;
}
void LiveLabel::sliderValueChanged (juce::Slider*)
{
    if (semiMode && ! timerStarted)
    {
        startTimer (100);
        timerStarted = true;
    }
    ticks = 0;
    char str[20];
    snprintf (str, 20, "%.2f", m_knob->slider()->getValue());
    m_knob->setText (str);
}
void LiveLabel::stopLive()
{
    ticks = 0;
    stopTimer();
    timerStarted = false;
}
LiveLabel::~LiveLabel()
{
    m_knob->slider()->removeListener (this);
}
void LiveLabel::sliderDragStarted (juce::Slider*)
{
    m_sliderDragging = true;
}
void LiveLabel::sliderDragEnded (juce::Slider*)
{
    m_sliderDragging = false;
}
} // namespace VeNo::GUIEvents