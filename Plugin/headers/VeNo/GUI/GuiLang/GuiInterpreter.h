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
    void parseMain (GUIParseItem* item);
    std::shared_ptr<ComponentGroup> parseTree (GUIParseItem* item, ComponentGroup* inGroup);
    std::shared_ptr<ComponentGroup> componentGroup = std::make_shared<ComponentGroup>();
    static tsl::robin_map<std::string, std::shared_ptr<ComponentFactory>> componentMapping;
    BaseComponent* find (const char* selector, ComponentGroup* inGroup = nullptr);

protected:
    // Yeah Regex! :(
    std::regex regex { "\\{(.*?)\\}", std::regex_constants::ECMAScript | std::regex_constants::icase };
    tsl::robin_map<std::string, std::string> parameters {};

    std::shared_ptr<BaseComponent> createComponent (GUIParseItem* item);
    std::string getParameterReplaced (std::string input);
    static std::shared_ptr<BaseComponent> createFromType (GUIParseItem* item, const std::string& parameter, const std::string& name, size_t id);
    static void initMapping();
};
} // namespace VeNo::GUI