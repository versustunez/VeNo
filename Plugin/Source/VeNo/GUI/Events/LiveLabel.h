#pragma once
#include <JuceHeader.h>
#include <VeNo/GUI/Components/Knob.h>

namespace VeNo::GUIEvents {
class LiveLabel : public juce::Slider::Listener, juce::Timer {
public:
  LiveLabel(GUI::Knob *knob, bool isSemi, std::string showName);
  ~LiveLabel() override;
  void stopLive();
  void sliderValueChanged(juce::Slider *slider) override;
  void sliderDragStarted(juce::Slider *slider) override;
  void sliderDragEnded(juce::Slider *slider) override;

private:
  void timerCallback() override;

protected:
  GUI::Knob *m_knob{nullptr};
  bool timerStarted{false};
  bool m_semiMode{false};
  int ticks{};
  std::string m_showName{};
  bool m_sliderDragging{false};
};
} // namespace VeNo::GUIEvents