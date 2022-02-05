#include <VeNo/GUI/ComponentFactories.h>
#include <VeNo/GUI/Components/Label.h>

namespace VeNo::GUI {
Ref<BaseComponent> LabelComponentFactory::create(GUIParseItem &item,
    const std::string &parameter, const std::string &name, size_t id,
    Interpreter *interpreter) {
  auto comp = CreateRef<Label>(parameter, name, id);
  comp->setText(item["text"]);
  doBase(comp.get(), item, interpreter);
  return comp;
}
} // namespace VeNo::GUI