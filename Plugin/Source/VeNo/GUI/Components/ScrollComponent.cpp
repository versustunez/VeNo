#include <VeNo/GUI/Components/ScrollComponent.h>
#include <utility>

namespace VeNo::GUI {

ScrollComponent::ScrollComponent(const std::string &name,
                                 const std::string &showName, size_t id)
    : BaseComponent(name, showName, id) {
  m_viewport = CreateRef<juce::Viewport>();
  addAndMakeVisible(m_viewport.get());
}

void ScrollComponent::setViewComponent(Ref<BaseComponent> component) {
  m_component = std::move(component);
  m_viewport->setViewedComponent(m_component.get(), false);
}
void ScrollComponent::resized() {
  if (m_viewport != nullptr) {
    m_viewport->setBounds(0, 0, getWidth(), getHeight());
    m_viewport->setViewPosition(0,0);
  }
}
} // namespace VeNo::GUI