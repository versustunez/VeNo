#pragma once
#include "BaseComponent.h"

namespace VeNo::GUI {
class Button : public BaseComponent, public juce::Button::Listener {
public:
  Button(const std::string &name, const std::string &showName, size_t id);
  ~Button() override;
  void setButtonText(const std::string &);
  void setIcon(const juce::String&);
  void resized() override;
  void setAction(std::string event);
  void buttonClicked(juce::Button *button) override;

  bool isFilled();
  void setFilled(bool isFilled);

  bool isIconButton();
  void setIsIconButton(bool isIconButton);

protected:
  Scope<juce::TextButton> m_button;
  std::string m_action;
  bool m_isFilledButton;
  bool m_isIconButton;
};
} // namespace VeNo::GUI