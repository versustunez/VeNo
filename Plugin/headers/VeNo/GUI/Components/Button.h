#pragma once
#include "BaseComponent.h"

namespace VeNo::GUI {
class Button : public BaseComponent, public juce::Button::Listener {
public:
  Button(const std::string &name, const std::string &showName, size_t id);
  ~Button() override;
  void setButtonText(const std::string &);
  void resized() override;
  void setAction(std::string event);
  void buttonClicked(juce::Button *button) override;

protected:
  Scope<juce::TextButton> m_button;
  std::string m_action;
};
} // namespace VeNo::GUI