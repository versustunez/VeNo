#pragma once
#include <JuceHeader.h>
#include <VeNo/TypeDefs.h>

namespace VeNo::GUI {
struct ColorChangeListener {
  virtual void HandleColorChange(const juce::Colour &color) = 0;
};
class ColorComponent : public juce::Component, public juce::ChangeListener {
public:
  ColorComponent(ColorChangeListener *listener,
                 const juce::Colour &currentColor);
  void resized() override;
  void changeListenerCallback(juce::ChangeBroadcaster *source) override;

protected:
  Scope<juce::ColourSelector> m_Selector;
  ColorChangeListener *m_ColorListener;
};
} // namespace VeNo::GUI
