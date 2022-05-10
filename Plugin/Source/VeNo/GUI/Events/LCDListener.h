#pragma once

#include <JuceHeader.h>
#include <VeNo/Events/EventHandler.h>

namespace VeNo::GUI {

class LCDListener : public juce::Slider::Listener {
public:
  LCDListener(InstanceID id);
  void sliderValueChanged(juce::Slider *slider) override;
  void sliderDragStarted(juce::Slider *slider) override;
  void sliderDragEnded(juce::Slider *slider) override;

protected:
  void init();
  InstanceID m_id{};
  Events::EventHandler *m_eventHandler{nullptr};
};
} // namespace VeNo::GUI