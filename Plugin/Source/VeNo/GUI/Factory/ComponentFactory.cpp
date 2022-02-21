#include <VeNo/GUI/ComponentFactories.h>
#include <VeNo/GUI/GuiLang/Interpreter.h>

namespace VeNo::GUI {
void ComponentFactory::setSelector(BaseComponent *component,
                                   GUIParseItem &item) {
  if (item.has("id"))
    component->setSelectorId(item.properties["id"]);
}
Ref<BaseComponent> ComponentFactory::create(GUIParseItem &, const std::string &,
                                            const std::string &, size_t,
                                            Interpreter *) {
  return nullptr;
}

void ComponentFactory::doBase(BaseComponent *component, GUIParseItem &item,
                              Interpreter *) {
  setSelector(component, item);
}
} // namespace VeNo::GUI