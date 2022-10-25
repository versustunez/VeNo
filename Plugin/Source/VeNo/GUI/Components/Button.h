#pragma once
#include "BaseComponent.h"

#include <optional>

namespace VeNo::GUI {
class Button : public BaseComponent, public juce::Button::Listener {
public:
  Button(const std::string &name, const std::string &showName, InstanceID id);
  Button(InstanceID id);
  ~Button() override;
  void setButtonText(const std::string &);
  void setIcon(const juce::String &);
  void resized() override;
  void setAction(std::string event);
  juce::TextButton *GetButton() { return m_button.get(); }
  std::string &action() { return m_action; }
  void buttonClicked(juce::Button *button) override;

  bool isFilled();
  void setFilled(bool isFilled);

  bool isIconButton();
  void setIsIconButton(bool isIconButton);

  void SetCallback(const std::function<void()> &callback) {
    m_Callback = callback;
  }

protected:
  std::optional<std::function<void()>> m_Callback{};
  Scope<juce::TextButton> m_button;
  std::string m_action;
  bool m_isFilledButton{false};
  bool m_isIconButton{false};
};
} // namespace VeNo::GUI