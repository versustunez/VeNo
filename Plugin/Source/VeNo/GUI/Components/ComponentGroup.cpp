#include <VeNo/Core/Config.h>
#include <VeNo/GUI/Components/ComponentGroup.h>
#include <VeNo/GUI/GUIUtils.h>
#include <utility>

namespace VeNo::GUI {
ComponentGroup::~ComponentGroup() { flex = nullptr; }
void ComponentGroup::paint(juce::Graphics &g) {
  auto *theme = Core::Config::get().theme().get();
  if (m_color.hasColor) {
    if (m_color.isPreColor)
      g.setColour(theme->getColor(m_color.preColor));
    else
      g.setColour(juce::Colour(m_color.colors[0], m_color.colors[1],
                               m_color.colors[2], m_color.colors[3]));
    g.fillAll();
  }
  if (m_showGroupName) {
    g.setColour(theme->getColor(Theme::Colors::font));
    int lineHeight = 16.0;
    g.drawText(m_showName, 0, 1, getWidth(), lineHeight + 2,
               juce::Justification::centred);
  }
}
void ComponentGroup::resized() {
  if (flex != nullptr) {
    flex->clear();
    flex->setItems(components, groups);
    flex->perform(getLocalBounds().toFloat());
    return;
  }
  int yOff = 0;
  if (m_showGroupName)
    yOff = 18;
  for (auto &subGroup : groups)
    subGroup->setBounds(subGroup->m_pos.x, subGroup->m_pos.y + yOff,
                        subGroup->m_pos.w, subGroup->m_pos.h);
  for (auto &child : components)
    child->setBounds(child->pos.x, child->pos.y + yOff, child->pos.w,
                     child->pos.h);
}
void ComponentGroup::showChildren() {
  for (auto &subGroup : groups) {
    addAndMakeVisible(*subGroup);
    subGroup->showChildren();
  }
  for (auto &child : components)
    addAndMakeVisible(child.get());
}
void ComponentGroup::setColor(const GUIColorComponent &color) {
  m_color = color;
}
void ComponentGroup::setPosition(Position pos) { m_pos = pos; }
Position &ComponentGroup::position() { return m_pos; }
void ComponentGroup::setShowName(std::string name) {
  m_showName = std::move(name);
  m_showGroupName = true;
}
std::string ComponentGroup::id() { return m_selector_id; }

void ComponentGroup::setSelectorId(std::string name) {
  m_selector_id = std::move(name);
}
} // namespace VeNo::GUI