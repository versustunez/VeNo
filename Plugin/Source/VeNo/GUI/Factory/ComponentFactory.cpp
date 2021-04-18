#include <VeNo/GUI/ComponentFactories.h>

namespace VeNo::GUI
{
std::shared_ptr<BaseComponent> ComponentFactory::create (GUIParseItem*, const std::string&, const std::string&, size_t)
{
    return nullptr;
}
} // namespace VeNo::GUI