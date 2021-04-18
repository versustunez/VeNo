#include <VUtils/Logging.h>
#include <VeNo/GUI/ComponentFactories.h>
#include <VeNo/GUI/Components/Knob.h>

namespace VeNo::GUI
{
std::shared_ptr<BaseComponent> KnobComponentFactory::create (GUIParseItem* item,
                                                             const std::string& parameter,
                                                             const std::string& name,
                                                             size_t id)
{
    auto comp = std::make_shared<Knob> (parameter, name, id);
    if (item->properties.contains ("label"))
    {
        auto& label = item->properties["label"];
        comp->setLabelPosition (label);
    }
    if (item->properties.contains ("labelvalue") && item->properties["labelvalue"] == "real")
    {
        comp->enableLiveLabel();
    }
    return comp;
}
} // namespace VeNo::GUI