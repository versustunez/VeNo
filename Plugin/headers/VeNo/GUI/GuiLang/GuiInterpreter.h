#pragma once

#include "Structs.h"
#include <VeNo/GUI/ComponentFactories.h>
#include <VeNo/GUI/Components/BaseComponent.h>
#include <VeNo/GUI/Components/ComponentGroup.h>
#include <memory>
#include <regex>

namespace VeNo::GUI
{
// Use the Generated Layout and Create some GUIs out of it! :D
class Interpreter
{
public:
    size_t m_id;
    explicit Interpreter (size_t id);
    ~Interpreter();
    void parseMain (GUIParseItem* item);
    std::shared_ptr<ComponentGroup> parseTree (GUIParseItem* item, ComponentGroup* inGroup);
    std::shared_ptr<ComponentGroup> componentGroup = std::make_shared<ComponentGroup>();
    static std::unordered_map<std::string, std::shared_ptr<ComponentFactory>> factories;
    BaseComponent* find (const char* selector, ComponentGroup* inGroup = nullptr);
    Events::EventHandler* eventHandler();
    static bool contains(const char* name, GUIParseItem* item);

protected:
    // Yeah Regex! :(
    std::regex regex { "\\{(.*?)\\}", std::regex_constants::ECMAScript | std::regex_constants::icase };
    std::unordered_map<std::string, std::string> parameters {};

    std::shared_ptr<BaseComponent> createComponent (GUIParseItem* item);
    std::string getParameterReplaced (std::string input);
    std::shared_ptr<BaseComponent> createFromType (GUIParseItem* item, const std::string& parameter, const std::string& name, size_t id);
    static void initMapping();
    Events::EventHandler* m_eventHandler = new Events::EventHandler(m_id);
};
} // namespace VeNo::GUI