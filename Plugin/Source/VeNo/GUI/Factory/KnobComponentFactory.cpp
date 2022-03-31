#include <VUtils/Logging.h>
#include <VeNo/GUI/ComponentFactories.h>
#include <VeNo/GUI/Components/Knob.h>

namespace VeNo::GUI {
Ref<BaseComponent> KnobComponentFactory::create(GUIParseItem &item,
                                                const std::string &parameter,
                                                const std::string &name,
                                                InstanceID id,
                                                Interpreter *interpreter) {
  auto comp = CreateRef<Knob>(parameter, name, id);
  if (item.has("label")) {
    auto &label = item["label"];
    comp->setLabelPosition(label);
  }
  if (item.has("tooltip")) {
    comp->setTooltip(item["tooltip"] == "true");
  }
  comp->setIsValueBox(isValueBox);
  if (item.has("label-value")) {
    auto &labelVal = item["label-value"];
    if (labelVal != "off")
      comp->enableLiveLabel(labelVal == "semi");
  }
  if (isValueBox && item.has("full-width"))
    comp->enableFullWidth();
  doBase(comp.get(), item, interpreter);
  return comp;
}
KnobComponentFactory::KnobComponentFactory(bool _isValueBox)
    : isValueBox(_isValueBox) {}
} // namespace VeNo::GUI