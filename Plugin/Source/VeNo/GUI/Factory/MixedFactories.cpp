#include "VeNo/GUI/Components/ScrollComponent.h"
#include "VeNo/GUI/Components/WaveThumbnails.h"

#include <VeNo/GUI/ComponentFactories.h>
#include <VeNo/GUI/Components/Logo.h>
namespace VeNo::GUI {
Ref<BaseComponent> LogoFactory::create(GUIParseItem &item,
                                       const std::string &parameter,
                                       const std::string &name, size_t id,
                                       Interpreter *interpreter) {
  auto comp = CreateRef<Logo>(parameter, name, id);
  doBase(comp.get(), item, interpreter);
  return comp;
}

Ref<BaseComponent> WaveThumbnailsFactory::create(GUIParseItem & item,
                                                 const std::string &parameter,
                                                 const std::string &name,
                                                 size_t id, Interpreter *) {
  auto scrollComponent = CreateRef<ScrollComponent>(parameter, name, id);
  auto comp = CreateRef<WaveThumbnails>(parameter, name, id);
  comp->setSize(item.pos.w, item.pos.h);
  scrollComponent->setViewComponent(comp);
  return scrollComponent;
}
} // namespace VeNo::GUI