#include <VeNo/GUI/ComponentFactories.h>

namespace VeNo::GUI {
Ref<BaseComponent> WaveEditorFactory::create(GUIParseItem &item,
    const std::string &parameter, const std::string &name, size_t id,
    Interpreter *interpreter) {
  return ComponentFactory::create(item, parameter, name, id, interpreter);
}
} // namespace VeNo::GUI