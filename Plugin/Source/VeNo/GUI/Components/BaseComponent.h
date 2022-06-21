#pragma once
#include "../GuiLang/Structs.h"
#include "ComponentGroup.h"
#include "VComponent.h"
#include "VeNo/Core/Parameter/Parameter.h"

#include <JuceHeader.h>
#include <VeNo/Events/EventHandler.h>
#include <VeNo/GUI/Events/ModulateMouseOpen.h>
#include <VeNo/TypeDefs.h>

namespace VeNo::GUI {
enum LabelPosition { NO_LABEL = 0, BOTTOM, TOP, CENTER };

class Interpreter;

class BaseComponent : public VComponent {
public:
  BaseComponent(std::string name, std::string showName, InstanceID id);
  ~BaseComponent() override = default;

  void setSelectorId(std::string selector);
  std::string &name();
  std::string &showName();
  InstanceID id() const;
  std::string &selectorId();
  LabelPosition m_labelPosition = LabelPosition::BOTTOM;
  Position pos{};

  void createLabel(const std::string &text, bool visible);
  void removeLabel();
  void setText(const std::string &text);
  void triggerAfterParsing(Interpreter *);

  virtual void addChild(const Ref<ComponentGroup> &){};

  Position resize();
  void setLabelPosition(const std::string &pos);
  void setEventHandler(Events::EventHandler *);

  int precision();

  void mouseEnter(const juce::MouseEvent &event) override;
  void mouseExit(const juce::MouseEvent &event) override;

  // LOOK for direct children of this component
  template <class ClassType> ClassType *child() {
    auto &children = getChildren();
    for (auto &childComponent : children) {
      auto component = dynamic_cast<ClassType *>(childComponent);
      if (component)
        return component;
    }
    return nullptr;
  }

protected:
  virtual void afterParsing(Interpreter *);
  std::string m_name;
  std::string m_showName;
  Scope<juce::Label> m_label;
  InstanceID m_id;
  std::string m_selectorId;
  bool m_afterParsingCalled = false;
  Events::EventHandler *m_handler{nullptr};
  Core::Parameter *m_parameter{nullptr};
  Scope<GUIEvents::ModulateMouseOpen> m_mousePicker{nullptr};

protected:
  template <class T> Ref<T> Make(std::string name, std::string showName) {
    static_assert(std::is_base_of_v<BaseComponent, T>,
                  "Is not Base of VComponent");
    auto ref = CreateRef<T>(name, showName, m_id);
    addAndMakeVisible(*ref);
    return ref;
  }
};
} // namespace VeNo::GUI