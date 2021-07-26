#include <VeNo/GUI/GuiLang/GuiInterpreter.h>
namespace VeNo::GUI
{
void Interpreter::initMapping()
{
    componentMapping["Label"] = std::make_shared<LabelComponentFactory>();
    componentMapping["Knob"] = std::make_shared<KnobComponentFactory>();
    componentMapping["WaveEditor"] = std::make_shared<WaveEditorFactory>();
    /*    componentMapping["Slider"] = ComponentID::SLIDER;
    componentMapping["Select"] = ComponentID::SELECT;*/
}
}