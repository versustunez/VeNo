#pragma once

#include <VeNo/TypeDefs.h>
#include "BaseComponent.h"
#include "ComponentGroup.h"

namespace VeNo::GUI {
class NestedComponent : public BaseComponent {
public:
  NestedComponent(const std::string& name,const std::string& showName, InstanceID id);
  void addChild(const Ref<ComponentGroup> &param) override;

  void setWidthMode(bool isWidthMode) { m_isWidthMode = isWidthMode; }
  void resized() override;
  void resizeComponent();

protected:
  bool m_isWidthMode{true};
  Vector<Ref<ComponentGroup>> m_children;
};
}