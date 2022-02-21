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
                                    const std::string &name, size_t id,
                                    Interpreter *interpreter);
  static void setSelector(BaseComponent *component, GUIParseItem &item);
  static void doBase(BaseComponent *, GUIParseItem &, Interpreter *);
};

class LabelComponentFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &item, const std::string &parameter,
                            const std::string &name, size_t id,
                            Interpreter *interpreter) override;
};

class KnobComponentFactory : public ComponentFactory {
public:
  KnobComponentFactory() = default;
  explicit KnobComponentFactory(bool isValueBox);
  Ref<BaseComponent> create(GUIParseItem &item, const std::string &parameter,
                            const std::string &name, size_t id,
                            Interpreter *interpreter) override;
  bool isValueBox{false};
};

class WaveEditorFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &item, const std::string &parameter,
                            const std::string &name, size_t id,
                            Interpreter *interpreter) override;
};

class SwitchFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &item, const std::string &parameter,
                            const std::string &name, size_t id,
                            Interpreter *interpreter) override;
};

class ButtonFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &item, const std::string &parameter,
                            const std::string &name, size_t id,
                            Interpreter *interpreter) override;
};

class LogoFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &item, const std::string &parameter,
                            const std::string &name, size_t id,
                            Interpreter *interpreter) override;
};

class WaveThumbnailsFactory : public ComponentFactory {
public:
  Ref<BaseComponent> create(GUIParseItem &, const std::string &parameter,
                            const std::string &name, size_t id, Interpreter *) override;
};

} // namespace VeNo::GUI