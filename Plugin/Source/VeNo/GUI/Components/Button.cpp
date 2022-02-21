#include <VeNo/GUI/Components/Button.h>
#include <utility>

namespace VeNo::GUI {
Button::Button(const std::string &name, const std::string &showName, size_t id)
    : BaseComponent(name, showName, id) {
  m_button = std::make_unique<juce::TextButton>();
  addAndMakeVisible(*m_button);
}
Button::~Button() { m_button.reset(nullptr); }
void Button::setButtonText(const std::string &text) {
  m_button->setButtonText(text);
}
void Button::resized() { m_button->setBounds(0, 0, getWidth(), getHeight()); }
void Button::setAction(std::string event) { m_action = std::move(event); }
void Button::buttonClicked(juce::Button *) {
  // on click trigger event on instance eventHandler
}
bool Button::isFilled() {
  return m_isFilledButton;
}
void Button::setFilled(bool isFilled) {
  m_isFilledButton = isFilled;
}
bool Button::isIconButton() {
  return m_isIconButton;
}

void Button::setIsIconButton(bool isIconButton) {
  m_isIconButton = isIconButton;
}
void Button::setIcon(const juce::String& icon) {
  m_isIconButton = true;
  m_button->setButtonText(icon);
}
} // namespace VeNo::GUI