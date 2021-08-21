#include <VeNo/Core/Config.h>
#include <VeNo/Core/Instance.h>
#include <VeNo/PluginEditor.h>
#include <VeNo/Window/WaveEditor.h>
#include <VeNo/GUI/GUIUtils.h>

namespace VeNo::Windows
{
WaveEditorWindow::WaveEditorWindow (size_t id)
    : juce::DocumentWindow (createName (id),
                            juce::Colours::black,
                            juce::DocumentWindow::closeButton,
                            true),
      m_id (id)
{
    instance = VeNo::Core::Instance::get (m_id);
    auto& config = VeNo::Core::Config::get();

    // prepare V-GUI Lang to use WaveEditor
    interpreter.parseMain (config.guiInit.getOrCreate ("Bin::WaveEditor"));
    interpreter.componentGroup.get();
    auto& pos = interpreter.componentGroup->position();
    int w = pos.w > 0 ? pos.w : WAVEEDITOR_WIDTH;
    int h = pos.h > 0 ? pos.h : WAVEEDITOR_HEIGHT;
    // Add New Component and Prepare
    auto* editor = instance->mainInterpreter.componentGroup->findParentComponentOfClass<VeNoEditor>();
    setContentNonOwned (interpreter.componentGroup.get(), false);
    auto editorPos = editor->getPosition();
    setBounds(editorPos.x, editorPos.y, w, h);
    setAlwaysOnTop (false);
    setResizable (false, false);
    setUsingNativeTitleBar (false);
    setVisible (true);
    toFront(true);
    DBGN ("Create WaveEditor")
}
WaveEditorWindow::~WaveEditorWindow()
{
    DBGN ("Delete WaveEditor")
    instance->state.waveEditorWindow = nullptr;
}
void WaveEditorWindow::closeButtonPressed()
{
    DBGN ("Close WaveEditor")
    delete instance->state.waveEditorWindow;
}
std::string WaveEditorWindow::createName (size_t id)
{
    return "WaveEditor - VeNo [" + std::to_string (id + 1) + "]";
}
} // namespace VeNo::Windows