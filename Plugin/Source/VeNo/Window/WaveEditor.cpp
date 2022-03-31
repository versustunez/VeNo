#include <VeNo/Core/Config.h>
#include <VeNo/Core/Instance.h>
#include <VeNo/PluginEditor.h>
#include <VeNo/Window/WaveEditor.h>

namespace VeNo::Windows {
WaveEditorWindow::WaveEditorWindow(InstanceID id)
    : juce::DocumentWindow(createName(id), juce::Colours::black,
                           juce::DocumentWindow::closeButton, true),
      m_id(id) {
  instance = Core::Instance::get(m_id);
  auto &config = Core::Config::get();

  // prepare V-GUI Lang to use WaveEditor
  auto mainUIContent =
      config.properties()->asString("gui.wave", "Bin::WaveEditor");
  interpreter.parseMain(config.guiInit.getOrCreate(mainUIContent));
  auto &pos = interpreter.componentGroup->position();
  int w = pos.w > 0 ? pos.w : WAVEEDITOR_WIDTH;
  int h = pos.h > 0 ? pos.h : WAVEEDITOR_HEIGHT;
  // Add New Component and Prepare
  auto *editor =
      instance->mainInterpreter->componentGroup->parent<VeNoEditor>();
  setContentNonOwned(interpreter.componentGroup.get(), false);
  auto editorPos = editor->getPosition();
  setBounds(editorPos.x, editorPos.y, w, h);
  setAlwaysOnTop(false);
  setResizable(false, false);
  setUsingNativeTitleBar(true);
  juce::DocumentWindow::setVisible(true);
  toFront(true);
  DBGN("Create WaveEditor");
}
WaveEditorWindow::~WaveEditorWindow() {
  DBGN("Delete WaveEditor");
  std::string editor_closed =
      "wave-editor-closed_" +
      std::to_string(instance->waveHolder.currentTable());
  instance->eventHandler.triggerEvent(editor_closed, new Events::RemoveEvent());
  instance->state.waveEditorWindow = nullptr;
}
void WaveEditorWindow::closeButtonPressed() {
  DBGN("Close WaveEditor");
  instance->state.waveEditorWindow = nullptr;
}
std::string WaveEditorWindow::createName(InstanceID id) {
  return "WaveEditor - VeNo [" + std::to_string(id + 1) + "]";
}
} // namespace VeNo::Windows