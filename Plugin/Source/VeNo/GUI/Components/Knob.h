#pragma once

#include "BaseComponent.h"

#include <JuceHeader.h>

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

// Forwards
namespace VeNo::GUIEvents {
class LiveLabel;
}
namespace VeNo::GUI {
class Knob : public BaseComponent, public juce::Label::Listener {
public:
  Knob(std::string name, std::string showName, InstanceID id);
  ~Knob() override;
  // Component
  void paint(juce::Graphics &g) override;
  void resized() override;

  // Juce Label Listener
  void labelTextChanged(juce::Label *labelThatHasChanged) override;
  void editorShown(juce::Label *label, juce::TextEditor &editor) override;
  void editorHidden(juce::Label *label, juce::TextEditor &editor) override;
  void enableLiveLabel(bool isSemi);

  juce::Slider *slider();
  juce::Label *label();

  static juce::Slider::SliderStyle getSliderStyle();

  void setMinMax(double min, double max, double interval = 0) {
    m_slider->setRange(min, max, interval);
  }
  void setValue(double value) { m_slider->setValue(value); }
  void setDefaultValue(double defaultValue) {
    m_slider->setDoubleClickReturnValue(true, defaultValue);
  }
  double getValue() { return m_slider->getValue(); };

  void setTooltip(bool tooltip);
  void enableFullWidth() { m_isFullWidth = true; }
  void setIsValueBox(bool valueBox) {
    m_isValueBox = valueBox;
    if (valueBox)
      m_slider->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
  };
  [[nodiscard]] bool isValueBox() const { return m_isValueBox; }
  [[nodiscard]] bool isFullWidth() const { return m_isFullWidth; }

protected:
  Scope<juce::Slider> m_slider;
  Scope<SliderAttachment> m_attachment;
  Scope<GUIEvents::LiveLabel> m_liveLabel{nullptr};
  bool m_isValueBox{false};
  bool m_isFullWidth{false};
};
} // namespace VeNo::GUI