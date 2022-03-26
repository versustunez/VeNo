#include <VeNo/GUI/Components/ScrollComponent.h>
#include <utility>

namespace VeNo::GUI {

ScrollComponent::ScrollComponent(const std::string &name,
                                 const std::string &showName, size_t id)
    : BaseComponent(name, showName, id) {
  m_viewport = CreateRef<juce::Viewport>();
  m_viewport->setScrollOnDragEnabled(true);
  m_viewport->setScrollBarsShown(false, false, true, false);
  addAndMakeVisible(m_viewport.get());
}

void ScrollComponent::setViewComponent(Ref<BaseComponent> component) {
  m_component = std::move(component);
  m_viewport->setViewedComponent(m_component.get(), false);
}
void ScrollComponent::resized() {
  if (m_viewport != nullptr) {
    m_viewport->setBounds(0, 0, pos.w, pos.h);
    m_viewport->setViewPosition(0, 0);
  }

  if (m_nestedComponent) {
    m_nestedComponent->resizeComponent();
  }
}
void ScrollComponent::addChild(const Ref<ComponentGroup> &param) {
  if (m_component != nullptr)
    return;
  if (m_nestedComponent == nullptr) {
    m_nestedComponent = VComponent::create<NestedComponent>(m_id);
    setViewComponent(m_nestedComponent);
  }
  m_nestedComponent->addChild(param);
}
} // namespace VeNo::GUI