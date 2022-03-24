#include <VeNo/Core/Config.h>
#include <VeNo/GUI/Components/Tabbed.h>

namespace VeNo::GUI {

TabbedComponent::TabbedComponent(const std::string &name,
                                 const std::string &showName, size_t id)
    : BaseComponent(name, showName, id) {

  m_tab = CreateRef<juce::TabbedComponent>(
      juce::TabbedButtonBar::Orientation::TabsAtTop);
  m_tab->setOutline(0);
  addAndMakeVisible(m_tab.get());
}

void TabbedComponent::addChild(const Ref<ComponentGroup> &group) {
  bool found = false;
  for (auto & component : group->components) {
    auto* tab = component->as<TabComponent>();
    if (tab) {
      m_tab->addTab(tab->showName(), tab->color(), tab, false);
      found = true;
    }
  }
  if (found) {
    m_children.push_back(group);
    group->setSize(getWidth(), getHeight());
  }
}

void TabbedComponent::resized() { m_tab->setSize(getWidth(), getHeight()); }

TabComponent::TabComponent(const std::string &name, const std::string &showName,
                           size_t id)
    : BaseComponent(name, showName, id) {}

void TabComponent::addChild(const Ref<ComponentGroup> &group) {
  m_children.push_back(group);
  group->showChildren();
  group->setSize(getWidth(), getHeight());
  addAndMakeVisible(group.get());
}
void TabComponent::resized() {
  for (auto& child : m_children) {
    child->setSize(getWidth(), getHeight());
  }
}
} // namespace VeNo::GUI
