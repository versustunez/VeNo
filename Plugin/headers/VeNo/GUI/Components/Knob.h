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
  Knob(std::string name, std::string showName, size_t id);
  ~Knob();
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
  void mouseDown(const juce::MouseEvent &event) override;

  static juce::Slider::SliderStyle getSliderStyle();
  static void openPopupMenu();

protected:
  Scope<juce::Slider> m_slider;
  Scope<SliderAttachment> m_attachment;
  Scope<GUIEvents::LiveLabel> m_liveLabel{nullptr};
};
} // namespace VeNo::GUI