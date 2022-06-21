#pragma once

#include "BaseComponent.h"
#include "Button.h"

namespace VeNo::GUI {
class PresetComponent : public BaseComponent, public Events::Handler, public juce::Label::Listener {
public:
  PresetComponent(const std::string &name, const std::string &showName,
                  InstanceID id);

  ~PresetComponent();

  void resized() override;
  void init();
  void handle(Events::Event *event) override;
  void labelTextChanged(juce::Label *labelThatHasChanged) override;

protected:
  Ref<Button> m_PrevButton{};
  Ref<Button> m_NextButton{};
  Ref<Button> m_SaveButton{};
  Ref<Button> m_LibraryButton{};
};
}