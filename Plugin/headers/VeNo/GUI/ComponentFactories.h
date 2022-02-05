#pragma once
#include "VeNo/GUI/GuiLang/Structs.h"
#include <VeNo/GUI/Components/BaseComponent.h>
#include <VeNo/TypeDefs.h>

namespace VeNo::GUI {
class Interpreter;
class ComponentFactory {
public:
  virtual Ref<BaseComponent> create(GUIParseItem &item,
      const std::string &parameter, const std::string &name, size_t id,
      Interpreter *interpreter);
  static void setSelector(BaseComponent *component, GUIParseItem &item);
  static void doBase(BaseComponent *, GUIParseItem &, Interpreter *);
};

class LabelComponentFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &item, const std::string &parameter,
      const std::string &name, size_t id, Interpreter *interpreter) override;
};

class KnobComponentFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &item, const std::string &parameter,
      const std::string &name, size_t id, Interpreter *interpreter) override;
};

class WaveEditorFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &item, const std::string &parameter,
      const std::string &name, size_t id, Interpreter *interpreter) override;
};

class SwitchFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &item, const std::string &parameter,
      const std::string &name, size_t id, Interpreter *interpreter) override;
};

class ButtonFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &item, const std::string &parameter,
      const std::string &name, size_t id, Interpreter *interpreter) override;
};

class LogoFactory : public ComponentFactory {
public:
  Ref<VeNo::GUI::BaseComponent> create(GUIParseItem &item,
      const std::string &parameter, const std::string &name, size_t id,
      Interpreter *interpreter) override;
};

} // namespace VeNo::GUI