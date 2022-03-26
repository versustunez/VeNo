#include <VeNo/GUI/Components/SelectComponent.h>

namespace VeNo::GUI {

Select::Select(const std::string& name, const std::string& showName, size_t id)
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
}