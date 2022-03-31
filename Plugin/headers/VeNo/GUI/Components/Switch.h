#pragma once
#include "BaseComponent.h"
typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
namespace VeNo::GUI {

struct SwitchStateText {
  std::string onText{};
  std::string offText{};
  bool isCustom{false};
};
class Switch : public BaseComponent {
public:
  Switch(const std::string &name, const std::string &showName, InstanceID id);
  ~Switch() override;
  void setButtonText(const std::string &);
  void resized() override;

  void setOnOffText(const std::string &on, const std::string &off) {
    m_text.offText = off;
    m_text.onText = on;
    m_text.isCustom = true;
  }

  std::string &on() { return m_text.onText; }

  std::string &off() { return m_text.offText; }

  bool useCustom();

private:
  Scope<juce::ToggleButton> m_button;
  Scope<ButtonAttachment> m_attachment;
  SwitchStateText m_text{};
};
} // namespace VeNo::GUI