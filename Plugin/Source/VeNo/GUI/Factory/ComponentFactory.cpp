#include <VeNo/GUI/ComponentFactories.h>
#include <VeNo/GUI/GuiLang/GuiInterpreter.h>

namespace VeNo::GUI
{
void ComponentFactory::setSelector (BaseComponent* component, GUIParseItem* item)
{
    if (contains ("id", item))
        component->setSelectorId (item->properties["id"]);
}
std::shared_ptr<BaseComponent> ComponentFactory::create (GUIParseItem*, const std::string&, const std::string&, size_t id, Interpreter*)
{
    return nullptr;
}

void ComponentFactory::doBase (BaseComponent* component, GUIParseItem* item, Interpreter* interpreter)
{
    setSelector (component, item);
    component->setEventHandler (interpreter->eventHandler());
}
bool ComponentFactory::contains(const char* name, GUIParseItem* item)
{
    return item->properties.find(name) != item->properties.end();
}
} // namespace VeNo::GUI