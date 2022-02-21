#include "VeNo/GUI/Components/WaveEditor.h"

#include <VeNo/GUI/ComponentFactories.h>

namespace VeNo::GUI {
Ref<BaseComponent> WaveEditorFactory::create(GUIParseItem &item,
                                             const std::string &parameter,
                                             const std::string &name, size_t id,
                                             Interpreter *interpreter) {
  auto component = CreateRef<WaveEditor>(parameter, name, id);
  component->setColorComponent(item.colorComponent);
  doBase(component.get(), item, interpreter);
  return component;
}
} // namespace VeNo::GUI