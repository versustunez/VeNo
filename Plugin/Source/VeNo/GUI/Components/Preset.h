#pragma once

#include "BaseComponent.h"
#include "Button.h"

namespace VeNo::GUI {

class PresetSaveDialog : public BaseComponent {
public:
  PresetSaveDialog(InstanceID id);
  void resized() override;
  void paint(juce::Graphics &g) override;
  juce::DialogWindow* DialogWindow{nullptr};
protected:
  Scope<juce::TextEditor> m_TextEditor{};
  Scope<Button> m_Button{};
  Scope<Button> m_RandomButton{};

};

class PresetComponent : public BaseComponent,
                        public Events::Handler,
                        public juce::Label::Listener {
public:
  PresetComponent(const std::string &name, const std::string &showName,
                  InstanceID id);

  ~PresetComponent();

  void resized() override;
  void init();
  void handle(Events::Event *event) override;
  void labelTextChanged(juce::Label *labelThatHasChanged) override;

  void openLibrary();
  void openSaveDialog();

protected:
  Ref<Button> m_PrevButton{};
  Ref<Button> m_NextButton{};
  Ref<Button> m_SaveButton{};
  Ref<Button> m_LibraryButton{};
};
} // namespace VeNo::GUI