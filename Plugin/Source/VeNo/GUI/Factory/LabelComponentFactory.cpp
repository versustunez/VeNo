#include <VeNo/GUI/ComponentFactories.h>
#include <VeNo/GUI/Components/Label.h>

namespace VeNo::GUI
{
std::shared_ptr<BaseComponent> LabelComponentFactory::create (GUIParseItem* item,
                                                              const std::string& parameter,
                                                              const std::string& name,
                                                              size_t id)
{
    auto comp = std::make_shared<Label> (parameter, name, id);
    comp->setText (item->properties["text"]);
    return comp;
}
} // namespace VeNo::GUI