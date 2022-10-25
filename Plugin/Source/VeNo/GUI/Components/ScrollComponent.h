#pragma once
#include "BaseComponent.h"
#include "NestedComponent.h"
namespace VeNo::GUI {
class ScrollComponent : public BaseComponent {
public:
  ScrollComponent(const std::string &name, const std::string &showName,
                  InstanceID id);
  void setViewComponent(Ref<BaseComponent> component);
  void resized() override;
  void paint(juce::Graphics &g) override;

  BaseComponent *component() { return m_component.get(); }
  void addChild(const Ref<ComponentGroup> &param) override;
  void setWidthMode(bool isWidthMode) { m_isWidthMode = isWidthMode; }
  void setFillMode(bool fillMode) { m_Fill = fillMode; }

  void enableVerticalScrollbar();

protected:
  bool m_isWidthMode{true};
  bool m_Fill{false};
  Ref<BaseComponent> m_component;
  Ref<juce::Viewport> m_viewport;
  Ref<NestedComponent> m_nestedComponent;
};
} // namespace VeNo::GUI