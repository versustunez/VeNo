#include <VeNo/GUI/ComponentFactories.h>

namespace VeNo::GUI
{
std::shared_ptr<BaseComponent> ComponentFactory::create (GUIParseItem*, const std::string&, const std::string&, size_t)
{
    return nullptr;
}
void ComponentFactory::setSelector (BaseComponent* component, GUIParseItem* item)
{
    if (item->properties.contains("id")) {
        component->setSelectorId(item->properties["id"]);
    }
}
} // namespace VeNo::GUI