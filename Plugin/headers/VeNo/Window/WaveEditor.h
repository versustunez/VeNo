#pragma once
#include <JuceHeader.h>
#include <VeNo/GUI/GuiLang/GuiInterpreter.h>

namespace VeNo::Core
{
class Instance;
}
namespace VeNo::Windows
{
class WaveEditorWindow : public juce::DocumentWindow
{
public:
    explicit WaveEditorWindow (size_t id);
    ~WaveEditorWindow() override;
    void closeButtonPressed() override;
protected:
    VeNo::Core::Instance* instance;
    size_t m_id = 0;
    VeNo::GUI::Interpreter interpreter{m_id};
    static std::string createName(size_t id);
};
} // namespace VeNo::Windows