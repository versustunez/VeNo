#include "ColorComponent.h"
namespace VeNo::GUI {
ColorComponent::ColorComponent(ColorChangeListener *listener,
                               const juce::Colour &currentColor)
    : m_ColorListener(listener) {
  typedef juce::ColourSelector::ColourSelectorOptions ColourSelectorOptions;
  int flags = ColourSelectorOptions::showColourspace |
              ColourSelectorOptions::showColourAtTop |
              ColourSelectorOptions::editableColour;
  m_Selector = CreateScope<juce::ColourSelector>(flags, 0, 0);
  m_Selector->setCurrentColour(currentColor,
                               juce::NotificationType::dontSendNotification);
  m_Selector->addChangeListener(this);
  addAndMakeVisible(*m_Selector);
}

void ColorComponent::resized() {
  m_Selector->setBounds(0, 0, getWidth(), getHeight());
}
void ColorComponent::changeListenerCallback(juce::ChangeBroadcaster *) {
  m_ColorListener->HandleColorChange(m_Selector->getCurrentColour());
}
} // namespace VeNo::GUI
