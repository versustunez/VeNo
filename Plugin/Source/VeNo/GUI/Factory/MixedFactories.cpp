#include <VeNo/GUI/ComponentFactories.h>
#include <VeNo/GUI/Components/Logo.h>
#include <VeNo/GUI/Components/ScrollComponent.h>
#include <VeNo/GUI/Components/Tabbed.h>
#include <VeNo/GUI/Components/WaveEditor.h>
#include <VeNo/GUI/Components/WaveThumbnails.h>

namespace VeNo::GUI {
Ref<BaseComponent> LogoFactory::create(GUIParseItem &item,
                                       const std::string &parameter,
                                       const std::string &name, size_t id,
                                       Interpreter *interpreter) {
  auto comp = CreateRef<Logo>(parameter, name, id);
  doBase(comp.get(), item, interpreter);
  return comp;
}

Ref<BaseComponent> WaveEditorFactory::create(GUIParseItem &item,
                                             const std::string &parameter,
                                             const std::string &name, size_t id,
                                             Interpreter *interpreter) {
  auto component = CreateRef<WaveEditor>(parameter, name, id);
  component->setColorComponent(item.colorComponent);
  doBase(component.get(), item, interpreter);
  return component;
}

Ref<BaseComponent> WaveThumbnailsFactory::create(GUIParseItem &item,
                                                 const std::string &parameter,
                                                 const std::string &name,
                                                 size_t id, Interpreter *) {
  auto scrollComponent = CreateRef<ScrollComponent>(parameter, name, id);
  scrollComponent->setSelectorId("WaveThumbnails");
  auto comp = CreateRef<WaveThumbnails>(parameter, name, id);
  comp->setSize(item.pos.w, item.pos.h);
  comp->createThumbnails();
  scrollComponent->setViewComponent(comp);
  return scrollComponent;
}

Ref<BaseComponent> TabbedFactory::create(GUIParseItem &,
                                         const std::string &parameter,
                                         const std::string &name, size_t id,
                                         Interpreter *) {
  return CreateRef<TabbedComponent>(parameter, name, id);
}

Ref<BaseComponent> TabFactory::create(GUIParseItem &item,
                                      const std::string &parameter,
                                      const std::string &name, size_t id,
                                      Interpreter *) {
  auto tab = CreateRef<TabComponent>(parameter, name, id);
  auto theme = Core::Config::get().theme().get();
  tab->setColor(item.colorComponent.isPreColor
                    ? theme->getColor(item.colorComponent.preColor)
                    : item.colorComponent.color);
  return tab;
}
} // namespace VeNo::GUI