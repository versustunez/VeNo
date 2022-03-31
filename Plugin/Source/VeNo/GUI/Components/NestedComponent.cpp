#include <VeNo/GUI/Components/NestedComponent.h>

namespace VeNo::GUI {
NestedComponent::NestedComponent(const std::string &name,
                                 const std::string &showName, InstanceID id)
    : BaseComponent(name, showName, id) {}

void NestedComponent::addChild(const Ref<ComponentGroup> &param) {
  m_children.push_back(param);
  param->showChildren();
  addAndMakeVisible(param.get());
  resizeComponent();
}
void NestedComponent::resizeComponent() {
  if (m_children.empty())
    return;
  for (auto &child : m_children) {
    auto _pos = child->getDirectChildrenSize();
    auto &groupPos = child->position();
    if (groupPos.w < _pos.w)
      groupPos.w = _pos.w;
    if (groupPos.h < _pos.h)
      groupPos.h = _pos.h;
    if (pos.w < groupPos.w)
      pos.w = groupPos.w;
    if (pos.h < groupPos.h)
      pos.h = groupPos.h;
  }
  setSize(pos.w, pos.h);
}
void NestedComponent::resized() {
  if (m_children.empty())
    return;
  for (auto &child : m_children) {
    auto _pos = child->getDirectChildrenSize();
    child->setSize(_pos.w, _pos.h);
  }
}
} // namespace VeNo::GUI