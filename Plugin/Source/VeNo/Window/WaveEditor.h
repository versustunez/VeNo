#pragma once
#include <JuceHeader.h>
#include <VeNo/GUI/GuiLang/Interpreter.h>

namespace VeNo::Core {
class Instance;
}
namespace VeNo::Windows {
class WaveEditorWindow : public juce::DocumentWindow {
public:
  explicit WaveEditorWindow(InstanceID id);
  ~WaveEditorWindow() override;
  void closeButtonPressed() override;

protected:
  Core::Instance *instance;
  InstanceID m_id = 0;
  GUI::Interpreter interpreter{m_id};
  static std::string createName(InstanceID id);
};
} // namespace VeNo::Windows