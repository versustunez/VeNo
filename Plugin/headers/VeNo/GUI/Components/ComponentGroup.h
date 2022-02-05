#pragma once

#include "BaseComponent.h"
#include <JuceHeader.h>
#include <VUtils/Logging.h>
#include <VeNo/GUI/Flex.h>
#include <VeNo/GUI/GuiLang/Structs.h>
#include <VeNo/TypeDefs.h>

namespace VeNo::GUI {
class ComponentGroup : public VComponent {
public:
  ~ComponentGroup() override;
  // Components can have Sub-Groups
  std::vector<Ref<ComponentGroup>> groups;
  // List of all Components of the current groups
  std::vector<Ref<BaseComponent>> components;
  void paint(juce::Graphics &g) override;
  void resized() override;
  void showChildren();
  void setColor(const GUIColorComponent &color);
  void setPosition(Position pos);
  void setShowName(std::string name);
  void setSelectorId(std::string id);
  std::string id();
  Position &position();
  Scope<Flex> flex{nullptr};

protected:
  GUIColorComponent m_color;
  Position m_pos;
  std::string m_showName;
  bool m_showGroupName = false;
  std::string m_selector_id;
};
} // namespace VeNo::GUI