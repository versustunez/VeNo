#include "ColorChangeComponent.h"

#include "VeNo/Core/Instance.h"

namespace VeNo::GUI {
ColorChangeComponent::ColorChangeComponent(const std::string &name,
                                           const std::string &showName,
                                           InstanceID id)
    : BaseComponent(name, showName, id) {
  m_config = &Core::Config::get();
  m_theme = m_config->theme().get();
  init();
  addMouseListener(this, true);
}

void ColorChangeComponent::init() {
  m_label = VComponent::create<Label>(m_id);
  m_label->setText(m_showName);
  m_label->setAlign("verticallyCentred");
  addAndMakeVisible(*m_label);
}

void ColorChangeComponent::paint(juce::Graphics &g) {
  g.setColour(juce::Colour(125, 125, 125).withAlpha(0.15f));
  g.fillRect(0, 5, getWidth(), getHeight() - 10);
  g.setColour(m_theme->getColor(m_color));
  float x = getWidth() - 90;
  float y = (getHeight() - 30) / 2.0;
  g.fillRoundedRectangle(x, y, 60, 30, 5);
}

void ColorChangeComponent::resized() {
  m_label->setBounds(20, 0, getWidth() - 20, getHeight());
}

void ColorChangeComponent::HandleColorChange(const juce::Colour &color) {
  m_theme->setColor(m_color, new juce::Colour(color));
  // Trigger that all Logo creations are done ;)
  if (m_color == Theme::Colors::logo || m_color == Theme::Colors::logoAccent) {
    Core::Instance::TriggerEventOnAllInstances("logo");
  } else {
    m_config->repaintEditors();
  }
}
void ColorChangeComponent::mouseDown(const juce::MouseEvent &) {
  Scope<ColorComponent> colorComponent =
      CreateScope<ColorComponent>(this, m_theme->getColor(m_color));
  colorComponent->setBounds(0, 0, 200, 300);
  auto rect = getScreenBounds();
  (void)juce::CallOutBox::launchAsynchronously(std::move(colorComponent), rect,
                                               nullptr);
}
} // namespace VeNo::GUI