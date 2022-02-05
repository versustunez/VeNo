#include <VeNo/GUI/GuiLang/Interpreter.h>
namespace VeNo::GUI {
void Interpreter::initMapping() {
  factories["Label"] = CreateRef<LabelComponentFactory>();
  factories["Knob"] = CreateRef<KnobComponentFactory>();
  factories["WaveEditor"] = CreateRef<WaveEditorFactory>();
  factories["Switch"] = CreateRef<SwitchFactory>();
  factories["Button"] = CreateRef<ButtonFactory>();
  factories["Logo"] = CreateRef<LogoFactory>();
  /*    componentMapping["Slider"] = ComponentID::SLIDER;
  componentMapping["Select"] = ComponentID::SELECT;*/
}
} // namespace VeNo::GUI