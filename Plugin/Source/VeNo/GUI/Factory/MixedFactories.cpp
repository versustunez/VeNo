#include "VeNo/GUI/Components/Preset.h"
#include "VeNo/GUI/Components/WaveForm.h"

#include <VeNo/GUI/ComponentFactories.h>
#include <VeNo/GUI/Components/Config/ConfigComponent.h>
#include <VeNo/GUI/Components/LCD/LCD.h>
#include <VeNo/GUI/Components/Logo.h>
#include <VeNo/GUI/Components/ScrollComponent.h>
#include <VeNo/GUI/Components/SelectComponent.h>
#include <VeNo/GUI/Components/Tabbed.h>

namespace VeNo::GUI {
Ref<BaseComponent> LogoFactory::create(GUIParseItem &item,
                                       const std::string &parameter,
                                       const std::string &name, InstanceID id,
                                       Interpreter *interpreter) {
  auto comp = CreateRef<Logo>(parameter, name, id);
  doBase(comp.get(), item, interpreter);
  return comp;
}

Ref<BaseComponent> ScrollComponentFactory::create(GUIParseItem &item,
                                                  const std::string &parameter,
                                                  const std::string &name,
                                                  InstanceID id,
                                                  Interpreter *) {

  auto comp = CreateRef<ScrollComponent>(parameter, name, id);
  if (item.has("axis")) {
    comp->setWidthMode(item["axis"] == "x");
  }
  return comp;
}

Ref<BaseComponent> TabbedFactory::create(GUIParseItem &,
                                         const std::string &parameter,
                                         const std::string &name, InstanceID id,
                                         Interpreter *) {
  return CreateRef<TabbedComponent>(parameter, name, id);
}

Ref<BaseComponent> TabFactory::create(GUIParseItem &item,
                                      const std::string &parameter,
                                      const std::string &name, InstanceID id,
                                      Interpreter *) {
  auto tab = CreateRef<TabComponent>(parameter, name, id);
  auto theme = Core::Config::get().theme().get();
  tab->setColor(item.colorComponent.isPreColor
                    ? theme->getColor(item.colorComponent.preColor)
                    : item.colorComponent.color);
  return tab;
}

Ref<BaseComponent> SelectFactory::create(GUIParseItem &item,
                                         const std::string &parameter,
                                         const std::string &name, InstanceID id,
                                         Interpreter *) {

  auto comp = CreateRef<Select>(parameter, name, id);
  if (item.has("label") && item["label"] == "no") {
    comp->removeLabel();
  }
  if (item.has("values")) {
    auto values = VUtils::StringUtils::split(item["values"], ",");
    for (auto &value : values) {
      comp->addItem(value);
    }
  } else if (item.has("preset")) {
    auto &preset = item["preset"];
    comp->setupPreset(preset);
  }
  comp->createAttachment();
  return comp;
}

Ref<BaseComponent> GroupFactory::create(GUIParseItem &,
                                        const std::string &parameter,
                                        const std::string &name, InstanceID id,
                                        Interpreter *) {
  return CreateRef<NestedComponent>(parameter, name, id);
}

Ref<BaseComponent> WaveFormFactory::create(GUIParseItem &item,
                                           const std::string &parameter,
                                           const std::string &name,
                                           InstanceID id, Interpreter *) {
  auto waveform = CreateRef<WaveFormComponent>(parameter, name, id);
  if (item.has("index"))
    waveform->init(VUtils::StringUtils::toNumber(item["index"], 0));
  return waveform;
}

Ref<BaseComponent> LCDFactory::create(GUIParseItem &,
                                      const std::string &parameter,
                                      const std::string &name, InstanceID id,
                                      Interpreter *) {
  return CreateRef<LCDComponent>(parameter, name, id);
}

Ref<BaseComponent> ConfigFactory::create(GUIParseItem & item,
                                      const std::string &parameter,
                                      const std::string &name, InstanceID id,
                                      Interpreter *) {

  auto scrollComponent = CreateRef<ScrollComponent>(parameter, name, id);
  scrollComponent->setSelectorId("ConfigFactory");
  auto comp = CreateRef<ConfigComponent>(parameter, name, id);
  comp->setSize(item.pos.w, item.pos.h);
  comp->calculateSize(item.pos.w);
  scrollComponent->setViewComponent(comp);
  scrollComponent->enableVerticalScrollbar();
  return scrollComponent;
}

Ref<BaseComponent> PresetFactory::create(GUIParseItem &,
                                         const std::string &parameter,
                                         const std::string &name, InstanceID id,
                                         Interpreter *) {
  auto presetComponent = CreateScope<PresetComponent>(parameter, name, id);
  presetComponent->init();
  return presetComponent;
}

} // namespace VeNo::GUI