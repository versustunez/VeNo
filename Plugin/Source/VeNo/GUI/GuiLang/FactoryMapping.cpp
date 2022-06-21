#include <VeNo/GUI/GuiLang/Interpreter.h>
namespace VeNo::GUI {
void Interpreter::initMapping() {
  factories["Label"] = CreateRef<LabelComponentFactory>();
  factories["Knob"] = CreateRef<KnobComponentFactory>();
  factories["ValueBox"] = CreateRef<KnobComponentFactory>(true);
  factories["Switch"] = CreateRef<SwitchFactory>();
  factories["Button"] = CreateRef<ButtonFactory>();
  factories["Logo"] = CreateRef<LogoFactory>();
  factories["Tabbed"] = CreateRef<TabbedFactory>();
  factories["Tab"] = CreateRef<TabFactory>();
  factories["Select"] = CreateRef<SelectFactory>();
  factories["Scrollable"] = CreateRef<ScrollComponentFactory>();
  factories["Group"] = CreateRef<GroupFactory>();
  factories["WaveForm"] = CreateRef<WaveFormFactory>();
  factories["LCD"] = CreateRef<LCDFactory>();
  factories["Config"] = CreateRef<ConfigFactory>();
  factories["Preset"] = CreateRef<PresetFactory>();
}
} // namespace VeNo::GUI