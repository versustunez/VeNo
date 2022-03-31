#include <VeNo/Events/GUIEvents.h>
#include <VeNo/GUI/Components/Button.h>
#include <utility>

namespace VeNo::GUI {
Button::Button(const std::string &name, const std::string &showName, InstanceID id)
    : BaseComponent(name, showName, id) {
  m_button = std::make_unique<juce::TextButton>();
  m_button->addListener(this);
  addAndMakeVisible(*m_button);
}
Button::~Button() { m_button.reset(nullptr); }
void Button::setButtonText(const std::string &text) {
  m_isIconButton = false;
  m_button->setButtonText(text);
}
void Button::resized() { m_button->setBounds(0, 0, getWidth(), getHeight()); }
void Button::setAction(std::string event) { m_action = std::move(event); }
void Button::buttonClicked(juce::Button *) {
  if (!m_action.empty()) {
    m_handler->triggerEvent(m_action, new Events::ButtonClickedEvent(this));
  }
}
bool Button::isFilled() { return m_isFilledButton; }
void Button::setFilled(bool isFilled) { m_isFilledButton = isFilled; }
bool Button::isIconButton() { return m_isIconButton; }

void Button::setIsIconButton(bool isIconButton) {
  m_isIconButton = isIconButton;
}
void Button::setIcon(const juce::String &icon) {
  m_isIconButton = true;
  m_button->setButtonText(icon);
}
} // namespace VeNo::GUI