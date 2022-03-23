#pragma once

#include "Structs.h"

#include <VeNo/GUI/ComponentFactories.h>
#include <VeNo/GUI/Components/BaseComponent.h>
#include <VeNo/GUI/Components/ComponentGroup.h>
#include <VeNo/TypeDefs.h>
#include <regex>

namespace VeNo::GUI {
// Use the Generated Layout and Create some GUIs out of it! :D
class Interpreter {
public:
  size_t m_id;
  explicit Interpreter(size_t id);
  ~Interpreter() = default;
  void parseMain(GUIParseItem *item);
  Ref<ComponentGroup> parseTree(GUIParseItem *item, ComponentGroup *inGroup);
  Ref<ComponentGroup> componentGroup = CreateRef<ComponentGroup>();
  static std::unordered_map<std::string, Ref<ComponentFactory>> factories;
  BaseComponent *find(const char *selector, ComponentGroup *inGroup = nullptr);
  static bool contains(const char *name, GUIParseItem *item);

protected:
  Ref<BaseComponent> createComponent(GUIParseItem *item);
  Ref<BaseComponent> createFromType(GUIParseItem *item,
                                    const std::string &parameter,
                                    const std::string &name, size_t id);
  void triggerAfterParsing(ComponentGroup *inGroup = nullptr);
  static void initMapping();
};
} // namespace VeNo::GUI