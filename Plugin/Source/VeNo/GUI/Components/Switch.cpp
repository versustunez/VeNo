#include <VeNo/Core/Instance.h>
#include <VeNo/GUI/Components/Switch.h>
namespace VeNo::GUI {
Switch::Switch(const std::string &name, const std::string &showName, InstanceID id)
    : BaseComponent(name, showName, id) {
  auto *instance = Core::Instance::get(m_id);
  m_button = std::make_unique<juce::ToggleButton>();
  if (instance && instance->treeState) {
    auto state = instance->treeState;
    if (state->getParameter(m_name) != nullptr)
      m_attachment =
          std::make_unique<ButtonAttachment>(*state, m_name, *m_button);
  }
  addAndMakeVisible(*m_button);
}
void Switch::setButtonText(const std::string &text) {
  m_button->setButtonText(text);
}
Switch::~Switch() {
  m_attachment.reset(nullptr);
  m_button.reset(nullptr);
}
void Switch::resized() { m_button->setBounds(0, 0, getWidth(), getHeight()); }
bool Switch::useCustom() { return m_text.isCustom; }
} // namespace VeNo::GUI
