#include <VeNo/Core/Instance.h>
#include <VeNo/GUI/Components/SelectComponent.h>

namespace VeNo::GUI {
Select::Select(const std::string &name, const std::string &showName, InstanceID id)
    : BaseComponent(name, showName, id) {
  m_comboBox = CreateRef<juce::ComboBox>();
  addAndMakeVisible(m_comboBox.get());
  createLabel(m_showName, true);
  m_label->setJustificationType(juce::Justification::centred);
}
void Select::resized() {
  auto position = resize();
  m_comboBox->setBounds(position.x, position.y, position.w, position.h);
}

void Select::addItem(const std::string &value) {
  m_comboBox->addItem(value, m_lastAdded++);
}
void Select::createAttachment() {
  auto *instance = Core::Instance::get(m_id);
  if (instance && instance->treeState) {
    auto state = instance->treeState;
    auto* parameter = state->getParameter(m_name);
    if (parameter != nullptr) {
      m_attachment = CreateScope<ComboBoxAttachment>(*state, m_name, *m_comboBox);
    }
  }
}
void setupDetunePreset(Select& comboBox) {
  comboBox.addItem("Super");
  comboBox.addItem("VeNoX");
  comboBox.addItem("Up-Shift"); // Moves only up ;)
  comboBox.addItem("Experimental"); // Moves crazy?! xD
}

void Select::setupPreset(const VString& preset)  {
  if (preset == "detune") {
    setupDetunePreset(*this);
  } else {
    // UNKNOWN
  }
}
} // namespace VeNo::GUI