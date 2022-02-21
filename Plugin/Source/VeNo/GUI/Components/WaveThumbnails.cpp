#include "VeNo/Core/Instance.h"

#include <VeNo/GUI/Components/WaveThumbnails.h>
#include <VeNo/GUI/Fonts/Icons.h>

namespace VeNo::GUI {

WaveThumbnails::WaveThumbnails(const std::string &name,
                               const std::string &showName, size_t id)
    : BaseComponent(name, showName, id) {
  m_addButton = VComponent::create<Button>(m_id);
  m_addButton->setIcon(Icons::FontAwesome_Plus);
  m_addButton->pos.w = 20;
  m_addButton->pos.h = 20;
  m_components.push_back(m_addButton);
  addAndMakeVisible(m_addButton.get());
  setupFlex();
  updateSize();
}
void WaveThumbnails::resized() {
  Vector<Ref<ComponentGroup>> groups = {};
  m_flex.clear();
  m_flex.setItems(m_components, groups);
  m_flex.perform(juce::Rectangle<float>(0, 0, (float)getWidth(), (float)getHeight()));
}

void WaveThumbnails::updateSize() {
  Position size{};
  for (auto &component : m_components) {
    size.h += component->pos.h;
    if (size.w < component->pos.w) {
      size.w = component->pos.w;
    }
  }
  setSize(size.w, size.h);
}

void WaveThumbnails::setupFlex() {
  std::string direction = "column";
  std::string justify = "center";
  m_flex.setDirection(direction);
  m_flex.setAlignItems(justify);
}
} // namespace VeNo::GUI