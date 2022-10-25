#include "Matrix.h"

#include <VeNo/Core/Instance.h>
#include <utility>

#define MATRIX_ITEM_HEIGHT 60

namespace VeNo::GUI {
MatrixItemComponent::MatrixItemComponent(const std::string &name,
                                         const std::string &modulator,
                                         InstanceID id)
    : m_id(id),
      m_Key(modulator + name),
      m_Modulator(modulator),
      m_Name(name) {
  auto &matrix = VeNo::Core::Instance::get(m_id)->synthesizer->matrix();
  m_Label = CreateScope<juce::Label>();
  m_Label->setText(m_Modulator, juce::NotificationType::dontSendNotification);
  m_AddRemoveButton = VComponent::create<Button>(m_id);
  m_AddRemoveButton->GetButton()->addListener(this);

  m_AmountKnob = CreateRef<Knob>("", "Amount", m_id);
  auto *slider = m_AmountKnob->slider();
  slider->setRange(-1, 1, 0.01);
  slider->setDoubleClickReturnValue(true, 0, juce::ModifierKeys());

  if (!matrix.Has(m_Key)) {
    m_AddRemoveButton->setButtonText("Add");
    m_AmountKnob->setVisible(false);
  } else {
    m_AddRemoveButton->setButtonText("Remove");
    m_AmountKnob->setVisible(true);
    slider->setValue(matrix.GetAmount(m_Key),
                     juce::NotificationType::dontSendNotification);
  }
  slider->addListener(this);
  addAndMakeVisible(*m_Label);
  addAndMakeVisible(*m_AddRemoveButton);
  addChildComponent(*m_AmountKnob);
}

void MatrixItemComponent::buttonClicked(juce::Button *) {
  auto &matrix = VeNo::Core::Instance::get(m_id)->synthesizer->matrix();
  if (matrix.Has(m_Key)) {
    matrix.remove(m_Key);
    m_AddRemoveButton->setButtonText("Add");
    m_AmountKnob->setVisible(false);
    Core::Instance::get(m_id)->eventHandler.triggerEvent(
        "matrix-changed", new Events::MatrixChangeEvent(m_Key, true));
  } else {
    if (matrix.add(m_Modulator, m_Name)) {
      m_AddRemoveButton->setButtonText("Remove");
      m_AmountKnob->setVisible(true);
      Core::Instance::get(m_id)->eventHandler.triggerEvent(
          "matrix-changed", new Events::MatrixChangeEvent(m_Key, false));
    }
  }
  // This is for the BigMatrix to let it update ;)
}
void MatrixItemComponent::sliderValueChanged(juce::Slider *) {
  auto *instance = VeNo::Core::Instance::get(m_id);
  auto &matrix = instance->synthesizer->matrix();
  matrix.setAmount(m_Key, m_AmountKnob->getValue());
  auto key = fmt::format("matrix-{}-changed", m_Key);
  instance->eventHandler.triggerEvent(
      key, new Events::ValueChange(m_AmountKnob->getValue()));
}
void MatrixItemComponent::resized() {
  float width = (float)getWidth() / 3.0f;
  float x = 0;
  m_Label->setBounds(x, 0, width, getHeight());
  x += width;
  m_AmountKnob->setBounds(x, 0, width, getHeight());
  x += width;
  m_AddRemoveButton->setBounds(x, (getHeight() * 0.5) - 10, width, 20);
}
void MatrixItemComponent::paint(juce::Graphics &g) {
  auto theme = Core::Config::get().theme()->getColor(Theme::Colors::accent);
  g.setColour(theme);
  g.fillRect(0, getHeight() - 1, getWidth(), 1);
}

MatrixComponent::MatrixComponent(std::string name, InstanceID id)
    : BaseComponent("", "", id),
      m_componentName(std::move(name)) {

  m_Instance = VeNo::Core::Instance::get(id);
  m_Matrix = &m_Instance->synthesizer->matrix();
  auto &modulators = m_Matrix->Modulators();
  for (auto &modulator : modulators) {
    auto &modName = modulator->name();
    if (modName.empty())
      continue;
    addAndMakeVisible(*m_Items.emplace_back(
        CreateRef<MatrixItemComponent>(m_componentName, modName, m_id)));
  }
  setSize(300, m_Items.size() * MATRIX_ITEM_HEIGHT);
}
void MatrixComponent::resized() {
  int y = 0;
  for (auto &item : m_Items) {
    item->setBounds(10, y + 5, getWidth() - 20, MATRIX_ITEM_HEIGHT - 10);
    y += MATRIX_ITEM_HEIGHT;
  }
}
void MatrixComponent::paint(juce::Graphics &g) {
  auto theme = Core::Config::get().theme()->getColor(Theme::Colors::bg);
  g.fillAll(theme);
}

#undef MATRIX_ITEM_HEIGHT
} // namespace VeNo::GUI