#include <VeNo/GUI/GuiLang/Interpreter.h>
namespace VeNo::GUI {
void Interpreter::initMapping() {
  factories["Label"] = CreateRef<LabelComponentFactory>();
  factories["Knob"] = CreateRef<KnobComponentFactory>();
  factories["ValueBox"] = CreateRef<KnobComponentFactory>(true);
  factories["WaveEditor"] = CreateRef<WaveEditorFactory>();
  factories["WaveThumbnails"] = CreateRef<WaveThumbnailsFactory>();
  factories["Switch"] = CreateRef<SwitchFactory>();
  factories["Button"] = CreateRef<ButtonFactory>();
  factories["Logo"] = CreateRef<LogoFactory>();
  /* factories["Select"] = ComponentID::SELECT;*/
}
} // namespace VeNo::GUI