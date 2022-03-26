#include <VeNo/GUI/ComponentFactories.h>
#include <VeNo/GUI/Components/Switch.h>

namespace VeNo::GUI {
Ref<BaseComponent> SwitchFactory::create(GUIParseItem &item,
                                         const std::string &parameter,
                                         const std::string &name, size_t id,
                                         Interpreter *interpreter) {
  auto comp = CreateRef<Switch>(parameter, name, id);
  if (item.has("on") && item.has("off")) {
    comp->setOnOffText(item["on"], item["off"]);
  }
  if (item.has("noText")) {
    comp->setOnOffText("", "");
  }
  comp->setButtonText(item.has("text") ? item["text"] : name);
  doBase(comp.get(), item, interpreter);
  return comp;
}
} // namespace VeNo::GUI