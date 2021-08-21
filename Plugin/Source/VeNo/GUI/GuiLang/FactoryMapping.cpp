#include <VeNo/GUI/GuiLang/GuiInterpreter.h>
namespace VeNo::GUI
{
void Interpreter::initMapping()
{
    factories["Label"] = std::make_shared<LabelComponentFactory>();
    factories["Knob"] = std::make_shared<KnobComponentFactory>();
    factories["WaveEditor"] = std::make_shared<WaveEditorFactory>();
    factories["ToggleButton"] = std::make_shared<ToggleButtonFactory>();
    /*    componentMapping["Slider"] = ComponentID::SLIDER;
    componentMapping["Select"] = ComponentID::SELECT;*/
}
}