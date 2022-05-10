#pragma once

#include "BaseComponent.h"

#include <JuceHeader.h>

namespace VeNo::GUI {
class TabbedComponent : public BaseComponent {
public:
  TabbedComponent(const std::string &name, const std::string &showName,
                  InstanceID id);

  void addChild(const Ref<ComponentGroup> &group) override;
  void resized() override;

protected:
  Vector<Ref<ComponentGroup>> m_children;
  Ref<juce::TabbedComponent> m_tab;
};

class TabComponent : public BaseComponent {
public:
  TabComponent(const std::string &name, const std::string &showName,
               InstanceID id);

  void addChild(const Ref<ComponentGroup> &group) override;
  juce::Colour color() { return m_color; }
  void setColor(juce::Colour color) { m_color = color; }
  void resized() override;

protected:
  Vector<Ref<ComponentGroup>> m_children;
  juce::Colour m_color;
};
} // namespace VeNo::GUI