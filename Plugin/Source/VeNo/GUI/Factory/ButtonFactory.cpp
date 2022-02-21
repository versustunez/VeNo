#include <VeNo/GUI/ComponentFactories.h>
#include <VeNo/GUI/Components/Button.h>
#include <VeNo/GUI/GuiLang/Interpreter.h>

namespace VeNo::GUI {
Ref<BaseComponent> ButtonFactory::create(GUIParseItem &item,
                                         const std::string &parameter,
                                         const std::string &name, size_t id,
                                         Interpreter *interpreter) {
  auto comp = CreateRef<Button>(parameter, name, id);
  if (item.has("text"))
    comp->setButtonText(item["text"]);
  else
    comp->setButtonText("TextButton");

  if (item.has("action"))
    comp->setAction(item["action"]);
  if (item.has("filled"))
    comp->setFilled(item["filled"] == "true");
  doBase(comp.get(), item, interpreter);
  return comp;
}
} // namespace VeNo::GUI