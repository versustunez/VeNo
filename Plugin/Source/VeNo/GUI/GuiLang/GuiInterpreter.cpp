#include <VUtils/Logging.h>
#include <VeNo/Core/Config.h>
#include <VeNo/Core/Instance.h>
#include <VeNo/GUI/Components/Components.h>
#include <VeNo/GUI/GuiLang/GuiInterpreter.h>
#include <regex>
#include <vendor/tsl/robin_map.h>

namespace VeNo::GUI
{
tsl::robin_map<std::string, std::shared_ptr<ComponentFactory>> Interpreter::componentMapping = {};
Interpreter::Interpreter (size_t id) : m_id (id) {}
std::shared_ptr<ComponentGroup> Interpreter::parseTree (GUIParseItem* item, ComponentGroup* inGroup)
{
    if (item == nullptr)
        return nullptr;
    std::shared_ptr<ComponentGroup> group = std::make_shared<ComponentGroup>();
    group->setColor (item->colorComponent);
    group->setPosition (item->pos);
    if (! inGroup)
    {
        inGroup = group.get();
    }
    // Imports need a sub-interpreter! so here we go:
    if (item->component != nullptr)
    {
        // we have a component here ;)
        auto comp = createComponent (item);
        if (comp != nullptr)
        {
            comp->pos = item->pos;
            inGroup->components.push_back (comp);
        }
        return nullptr;
    }
    else
    {
        for (auto& import : item->imports)
        {
            auto importTree = VeNo::Core::Config::get().guiInit.get (import.name);
            if (importTree != nullptr)
            {
                auto interpreter = Interpreter (m_id);
                interpreter.parameters = import.params;
                auto importGroup = interpreter.parseTree (importTree, nullptr);
                for (auto& component : importGroup->components)
                    group->components.push_back (component);
                for (auto& importedGroup : importGroup->groups)
                    group->groups.push_back (importedGroup);
            }
        }

        for (auto& i : item->items)
        {
            auto parsed = parseTree (i, group.get());
            if (parsed != nullptr)
                group->groups.push_back (parsed);
        }
    }
    return group;
}
std::shared_ptr<BaseComponent> Interpreter::createComponent (GUIParseItem* item)
{
    auto instance = VeNo::Core::Instance::get (m_id);
    if (! instance)
    {
        ERR ("Cannot find Instance what happens here?")
        return nullptr;
    }
    std::string name;
    std::string param = getParameterReplaced (item->component->parameter);
    if (! item->name.empty())
        name = getParameterReplaced (item->name);
    else
        name = instance->handler->getShowName (param);

    return createFromType (item, param, name, m_id);
}
std::string Interpreter::getParameterReplaced (std::string input)
{
    auto words_begin = std::sregex_iterator (input.begin(), input.end(), regex);
    auto words_end = std::sregex_iterator();
    for (std::sregex_iterator i = words_begin; i != words_end; ++i)
    {
        std::smatch match = *i;
        std::string replace;
        std::string key = match[1].str();
        if (parameters.contains (key))
            replace = parameters[key];
        input.replace (input.find (match[0]), match[0].str().size(), replace);
    }
    return input;
}
void Interpreter::parseMain (GUIParseItem* item)
{
    auto parsed = parseTree (item, nullptr);
    if (parsed != nullptr)
        componentGroup = parsed;
    componentGroup->showChilds();
}

std::shared_ptr<BaseComponent> Interpreter::createFromType (GUIParseItem* item, const std::string& parameter, const std::string& name, size_t id)
{
    if (componentMapping.empty())
        initMapping();
    if (componentMapping.contains (item->component->name))
        return componentMapping[item->component->name]->create (item, parameter, name, id);
    return nullptr;
}
void Interpreter::initMapping()
{
    componentMapping["Label"] = std::make_shared<LabelComponentFactory>();
    componentMapping["Knob"] = std::make_shared<KnobComponentFactory>();
    /*    componentMapping["Slider"] = ComponentID::SLIDER;
    componentMapping["Select"] = ComponentID::SELECT;*/
}
} // namespace VeNo::GUI