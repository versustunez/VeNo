#pragma once
#include "VeNo/GUI/GuiLang/Structs.h"

#include <VeNo/GUI/Components/BaseComponent.h>
#include <VeNo/TypeDefs.h>

namespace VeNo::GUI {
class Interpreter;
class ComponentFactory {
public:
  virtual Ref<BaseComponent> create(GUIParseItem &item,
                                    const std::string &parameter,
                                    const std::string &name, InstanceID id,
                                    Interpreter *interpreter);
  static void setSelector(BaseComponent *component, GUIParseItem &item);
  static void doBase(BaseComponent *, GUIParseItem &, Interpreter *);
};

class LabelComponentFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &item, const std::string &parameter,
                            const std::string &name, InstanceID id,
                            Interpreter *interpreter) override;
};

class KnobComponentFactory : public ComponentFactory {
public:
  KnobComponentFactory() = default;
  explicit KnobComponentFactory(bool isValueBox);
  Ref<BaseComponent> create(GUIParseItem &item, const std::string &parameter,
                            const std::string &name, InstanceID id,
                            Interpreter *interpreter) override;
  bool isValueBox{false};
};

class SwitchFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &item, const std::string &parameter,
                            const std::string &name, InstanceID id,
                            Interpreter *interpreter) override;
};

class ButtonFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &item, const std::string &parameter,
                            const std::string &name, InstanceID id,
                            Interpreter *interpreter) override;
};

class LogoFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &item, const std::string &parameter,
                            const std::string &name, InstanceID id,
                            Interpreter *interpreter) override;
};

class ScrollComponentFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &, const std::string &parameter,
                            const std::string &name, InstanceID id,
                            Interpreter *) override;
};

class TabbedFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &, const std::string &parameter,
                            const std::string &name, InstanceID id,
                            Interpreter *) override;
};

class TabFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &, const std::string &parameter,
                            const std::string &name, InstanceID id,
                            Interpreter *) override;
};

class SelectFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &, const std::string &parameter,
                            const std::string &name, InstanceID id,
                            Interpreter *) override;
};

class GroupFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &, const std::string &parameter,
                            const std::string &name, InstanceID id,
                            Interpreter *) override;
};

class WaveFormFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &, const std::string &parameter,
                            const std::string &name, InstanceID id,
                            Interpreter *) override;
};

class LCDFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &, const std::string &parameter,
                            const std::string &name, InstanceID id,
                            Interpreter *) override;
};

class ConfigFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &, const std::string &parameter,
                            const std::string &name, InstanceID id,
                            Interpreter *) override;
};

class PresetFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &, const std::string &parameter,
                            const std::string &name, InstanceID id,
                            Interpreter *) override;
};

class FXListSortFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &, const std::string &parameter,
                            const std::string &name, InstanceID id,
                            Interpreter *) override;
};

} // namespace VeNo::GUI