#include <VUtils/Logging.h>
#include <VUtils/StringUtils.h>
#include <VeNo/Core/Config.h>
#include <VeNo/Core/Instance.h>
#include <VeNo/GUI/GuiLang/Interpreter.h>
#include <VeNo/Utils/ProfileMacros.h>
#include <regex>

namespace VeNo::GUI {
std::unordered_map<std::string, Ref<ComponentFactory>> Interpreter::factories =
    {};
Interpreter::Interpreter(InstanceID id) : m_id(id) {}
Ref<ComponentGroup> Interpreter::parseTree(GUIParseItem *item,
                                           ComponentGroup *inGroup) {
  if (item == nullptr)
    return nullptr;
#ifdef VENO_PROFILE
  auto name = item->name.empty() && item->component != nullptr
                  ? item->component->name + " | " + item->component->parameter
                  : item->name;
  VENO_PROFILE_SCOPE("Interpreter::parseTree > " + name);
#endif
  auto &guiItem = *item;
  Ref<ComponentGroup> group = CreateRef<ComponentGroup>();
  group->setColor(item->colorComponent);
  group->setPosition(item->pos);
  if (!inGroup)
    inGroup = group.get();
  // Imports need a sub-interpreter! so here we go:
  if (guiItem.component != nullptr) {
    // we have a component here ;)
    auto comp = createComponent(item);
    if (comp != nullptr) {
      comp->pos = item->pos;
      inGroup->components.push_back(comp);
      Ref<ComponentGroup> innerGroup = CreateRef<ComponentGroup>();
      for (auto &i : item->items) {
        auto parsed = parseTree(i, innerGroup.get());
        if (parsed != nullptr) {
          innerGroup->groups.push_back(parsed);
        }
      }
      comp->addChild(innerGroup);
    }
    return nullptr;
  } else {
    if (guiItem.has("flex") && guiItem["flex"] == "true")
      group->flex = CreateScope<Flex>(item->properties);
    if ((guiItem.has("id") && guiItem["id"] == "true") ||
        (guiItem.has("header") && guiItem["header"] == "true"))
      group->setShowName(item->name);

    for (auto &i : item->items) {
      auto parsed = parseTree(i, group.get());
      if (parsed != nullptr)
        group->groups.push_back(parsed);
    }
  }
  return group;
}
Ref<BaseComponent> Interpreter::createComponent(GUIParseItem *item) {
  VENO_PROFILE_FUNCTION();
  auto instance = Core::Instance::get(m_id);
  if (!instance) {
    ERR("Cannot find Instance what happens here?");
    return nullptr;
  }
  std::string name;
  std::string param = item->component->parameter;
  if (!item->name.empty())
    name = item->name;
  else
    name = instance->handler->getShowName(param);

  return createFromType(item, param, name, m_id);
}
void Interpreter::parseMain(GUIParseItem *item) {
  VENO_PROFILE_FUNCTION();
  if (componentGroup != nullptr)
    componentGroup.reset();
  auto parsed = parseTree(item, nullptr);
  if (parsed != nullptr)
    componentGroup = parsed;
  if (componentGroup == nullptr)
    return;
  triggerAfterParsing(componentGroup.get());
  componentGroup->showChildren();
}

Ref<BaseComponent> Interpreter::createFromType(GUIParseItem *item,
                                               const std::string &parameter,
                                               const std::string &name,
                                               InstanceID id) {
  VENO_PROFILE_FUNCTION();
  if (factories.empty())
    initMapping();
  if (factories.find(item->component->name) != factories.end())
    return factories[item->component->name]->create(*item, parameter, name, id,
                                                    this);
  WARN(R"(Found unknown Component: "{}")", item->component->name);
  return nullptr;
}

BaseComponent *Interpreter::find(const char *selector,
                                 ComponentGroup *inGroup) {
  VENO_PROFILE_FUNCTION();
  if (!inGroup)
    inGroup = componentGroup.get();
  auto selectors = VUtils::StringUtils::split(selector, " ");
  if (selectors.empty())
    return nullptr;

  for (auto &query : selectors) {
    // we have a group selector so lets search for a group :P
    if (query[0] == '#') {
      for (auto &group : inGroup->groups) {
        if (group->id() == query) {
          inGroup = group.get();
          break;
        }
      }
    } else {
      for (auto &component : inGroup->components)
        if (component->selectorId() == query)
          return component.get();

      for (auto &component : inGroup->groups) {
        auto *el = find(query.c_str(), component.get());
        if (el)
          return el;
      }
    }
  }
  return nullptr;
}
bool Interpreter::contains(const char *name, GUIParseItem *item) {
  return item->properties.find(name) != item->properties.end();
}
void Interpreter::removeUI() {
  if (componentGroup != nullptr)
    componentGroup.reset();
}
void Interpreter::triggerAfterParsing(ComponentGroup *inGroup) {
  VENO_PROFILE_FUNCTION();
  if (inGroup == nullptr)
    return;

  std::queue<ComponentGroup *> groups;

  groups.push(inGroup);
  while (!groups.empty()) {
    auto *rawGroup = groups.front();
    groups.pop();
    if (rawGroup == nullptr)
      continue;
    for (auto &group : rawGroup->groups)
      groups.push(group.get());

    for (auto &component : rawGroup->components)
      component->triggerAfterParsing(this);
  }
}
} // namespace VeNo::GUI
