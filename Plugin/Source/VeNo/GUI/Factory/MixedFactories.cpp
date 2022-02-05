#include <VeNo/GUI/ComponentFactories.h>
#include <VeNo/GUI/Components/Logo.h>
namespace VeNo::GUI {
Ref<BaseComponent> LogoFactory::create(GUIParseItem &item,
    const std::string &parameter, const std::string &name, size_t id,
    Interpreter *interpreter) {
  auto comp = CreateRef<Logo>(parameter, name, id);
  doBase(comp.get(), item, interpreter);
  return comp;
}
} // namespace VeNo::GUI