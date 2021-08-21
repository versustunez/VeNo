#include <VeNo/GUI/ComponentFactories.h>
#include <VeNo/GUI/Components/Button.h>
#include <VeNo/GUI/GuiLang/GuiInterpreter.h>

namespace VeNo::GUI
{
std::shared_ptr<BaseComponent> ButtonFactory::create (GUIParseItem* item, const std::string& parameter, const std::string& name, size_t id, Interpreter* interpreter)
{
    auto comp = std::make_shared<Button> (parameter, name, id);
    if (contains ("text", item))
        comp->setButtonText (item->properties["text"]);
    else
        comp->setButtonText ("TextButton");

    if (contains ("action", item))
        comp->setAction (item->properties["action"]);
    doBase (comp.get(), item, interpreter);
    return comp;
}
} // namespace VeNo::GUI