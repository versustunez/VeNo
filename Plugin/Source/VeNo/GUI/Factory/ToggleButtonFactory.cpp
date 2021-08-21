#include <VeNo/GUI/ComponentFactories.h>
#include <VeNo/GUI/Components/ToggleButton.h>

namespace VeNo::GUI
{
std::shared_ptr<BaseComponent> ToggleButtonFactory::create (GUIParseItem* item,
                                                            const std::string& parameter,
                                                            const std::string& name,
                                                            size_t id,
                                                            Interpreter* interpreter)
{
    auto comp = std::make_shared<ToggleButton> (parameter, name, id);
    if (contains ("text", item))
        comp->setButtonText (item->properties["text"]);
    else
        comp->setButtonText (name);
    doBase (comp.get(), item, interpreter);
    return comp;
}
} // namespace VeNo::GUI