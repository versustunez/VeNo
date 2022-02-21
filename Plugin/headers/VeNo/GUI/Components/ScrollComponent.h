#pragma once
#include "BaseComponent.h"
namespace VeNo::GUI {
class ScrollComponent : public BaseComponent {
public:
  ScrollComponent(const std::string &name, const std::string &showName,
                  size_t id);
  void setViewComponent(Ref<BaseComponent> component);
  void resized() override;

protected:
  Ref<BaseComponent> m_component;
  Ref<juce::Viewport> m_viewport;
};
} // namespace VeNo::GUI