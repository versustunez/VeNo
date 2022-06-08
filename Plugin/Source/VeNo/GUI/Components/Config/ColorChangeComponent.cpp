#include "ColorChangeComponent.h"

namespace VeNo::GUI {
ColorChangeComponent::ColorChangeComponent(const std::string &name,
                                           const std::string &showName,
                                           InstanceID id)
    : BaseComponent(name, showName, id) {
  m_config = &Core::Config::get();
  m_theme = m_config->theme().get();
  init();
}

void ColorChangeComponent::init() {
  m_label = VComponent::create<Label>(m_id);
  m_label->setText(m_showName);
  std::string align = "centred";
  m_label->setAlign(align);
  typedef juce::ColourSelector::ColourSelectorOptions ColourSelectorOptions;
  m_select = CreateScope<juce::ColourSelector>(
      ColourSelectorOptions::showColourspace |
          ColourSelectorOptions::showColourAtTop |
          ColourSelectorOptions::editableColour,
      0, 0);
  m_select->addChangeListener(this);
  addAndMakeVisible(*m_label);
  addAndMakeVisible(*m_select);
}

void ColorChangeComponent::paint(juce::Graphics &g) {
  auto mid = getWidth() / 2;
  auto xOf = mid / 2;
  auto color = juce::Colour(125, 125, 125).withAlpha(0.15f);
  g.setColour(color);
  g.fillRect(xOf, 0, mid, 30);
}

void ColorChangeComponent::resized() {
  m_label->setBounds(0, 0, getWidth(), 30);
  m_select->setBounds(getWidth() / 2 / 2, 30, getWidth() / 2, getHeight() - 40);
}

void ColorChangeComponent::changeListenerCallback(juce::ChangeBroadcaster *) {
  m_theme->setColor(m_color, new juce::Colour(m_select->getCurrentColour()));
  if (m_color == Theme::Colors::logo || m_color == Theme::Colors::logoAccent)
    m_handler->triggerEvent("logo", new Events::ChangeEvent());
  m_config->repaintEditors();
}
} // namespace VeNo::GUI