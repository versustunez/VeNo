#include <VUtils/Logging.h>
#include <VeNo/GUI/ComponentFactories.h>
#include <VeNo/GUI/Components/Knob.h>

namespace VeNo::GUI
{
std::shared_ptr<BaseComponent> KnobComponentFactory::create (GUIParseItem* item,
                                                             const std::string& parameter,
                                                             const std::string& name,
                                                             size_t id,
                                                             Interpreter* interpreter)
{
    auto comp = std::make_shared<Knob> (parameter, name, id);
    if (contains("label", item))
    {
        auto& label = item->properties["label"];
        comp->setLabelPosition (label);
    }
    if (contains("labelvalue", item))
    {
        auto& labelVal = item->properties["labelvalue"];
        if (labelVal != "off")
            comp->enableLiveLabel (labelVal == "semi");
    }
    doBase (comp.get(), item, interpreter);
    return comp;
}
} // namespace VeNo::GUI