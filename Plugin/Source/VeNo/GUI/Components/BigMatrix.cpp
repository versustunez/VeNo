#include "BigMatrix.h"

#include "VeNo/Core/Instance.h"

#include <utility>

namespace VeNo::GUI {

BigMatrixItem::BigMatrixItem(std::string matrixKey, InstanceID id)
    : BaseComponent("", "", id),
      m_MatrixKey(std::move(matrixKey)) {
  auto &matrix = Core::Instance::get(m_id)->synthesizer->matrix();
  m_handler->addHandler(std::format("matrix-{}-changed", m_MatrixKey), this);
  m_AmountKnob = CreateScope<Knob>("", "Amount", m_id);
  auto *slider = m_AmountKnob->slider();
  slider->setRange(-1, 1, 0.01);
  slider->setDoubleClickReturnValue(true, 0, juce::ModifierKeys());
  m_ActionButton = CreateScope<Button>(m_id);
  m_ActionButton->setButtonText("Remove");
  m_ActionButton->SetCallback([&] {
    matrix.remove(m_MatrixKey);
    m_handler->triggerEvent("matrix-changed",
                            new Events::MatrixChangeEvent(m_MatrixKey, true));
  });
  m_AmountKnob->setVisible(true);
  m_AmountKnob->enableLiveLabel(true);
  slider->setValue(matrix.GetAmount(m_MatrixKey),
                   juce::NotificationType::dontSendNotification);
  slider->addListener(this);
  addAndMakeVisible(*m_ActionButton);
  addChildComponent(*m_AmountKnob);
}

BigMatrixItem::~BigMatrixItem() {
  m_handler->removeHandler(std::format("matrix-{}-changed", m_MatrixKey));
}

void BigMatrixItem::paint(juce::Graphics &g) {
  auto &matrix = Core::Instance::get(m_id)->synthesizer->matrix();
  if (!matrix.Has(m_MatrixKey))
    return;
  auto &item = matrix.GetModulationItem(m_MatrixKey);
  double onePercent = getWidth() / 100.0;
  double onePercentH = getHeight() / 100.0;
  auto theme = Core::Config::get().theme();
  g.fillAll(theme->getColor(Theme::Colors::bg));
  g.setColour(theme->getColor(Theme::Colors::font));
  {
    auto source = std::format("Source: {}", item.Source->name());
    g.drawText(source, onePercent * 2, 0, onePercent * 50, onePercentH * 50,
               juce::Justification::bottomLeft);
  }
  {
    auto destination =
        std::format("Dest:   {}", item.Destination->getShowName());
    g.drawText(destination, onePercent * 2, onePercentH * 50, onePercent * 50,
               onePercentH * 50, juce::Justification::topLeft);
  }

  g.setColour(theme->getColor(Theme::Colors::accent));
  g.fillRect(0, getHeight() - 1, getWidth(), 1);
}

void BigMatrixItem::resized() {
  double onePercent = getWidth() / 100.0;
  double onePercentH = getHeight() / 100.0;
  m_AmountKnob->setBounds(onePercent * 55, onePercentH * 10, onePercent * 20,
                          onePercentH * 80);
  m_ActionButton->setBounds(onePercent * 77, onePercentH * 30, onePercent * 20,
                            onePercentH * 40);
}

void BigMatrixItem::handle(Events::Event *event) {
  auto *valueChangeEvent = event->as<Events::ValueChange>();
  if (!valueChangeEvent)
    return;
  m_AmountKnob->setValue(valueChangeEvent->value);
}

void BigMatrixItem::sliderValueChanged(juce::Slider *) {
  auto &matrix = Core::Instance::get(m_id)->synthesizer->matrix();
  matrix.setAmount(m_MatrixKey, m_AmountKnob->getValue());
}

BigMatrix::BigMatrix(const std::string &name, const std::string &showName,
                     InstanceID id)
    : BaseComponent(name, showName, id) {
  m_handler->addHandler("matrix-changed", this);
  CreateList();
}

BigMatrix::~BigMatrix() { m_handler->removeHandler("matrix-changed"); }

void BigMatrix::handle(Events::Event *ev) {
  // Triggered by Changing Presets
  if (ev->is<Events::ChangeEvent>()) {
    removeAllChildren();
    m_Items.clear();
    CreateList();
    return;
  }
  auto *event = ev->as<Events::MatrixChangeEvent>();
  if (!event)
    return;
  // Update current item list ;)
  if (event->IsDeleted) {
    if (m_Items.find(event->Key) != m_Items.end()) {
      auto &item = m_Items.at(event->Key);
      removeChildComponent(&(*item.get()));
      m_Items.erase(event->Key);
      UpdateSize();
    }
  } else {
    CreateList();
  }
}

void BigMatrix::UpdateSize() {
  int i = 0;
  constexpr int elementHeight = 70;
  for (auto &[key, value] : m_Items) {
    value->setBounds(0, i * elementHeight, getWidth(), elementHeight);
    i++;
  }
  setSize(getWidth(), m_Items.size() * elementHeight + 30);
}

void BigMatrix::CreateList() {
  auto &matrix = Core::Instance::get(m_id)->synthesizer->matrix();
  auto &items = matrix.GetItems();
  for (auto &[key, value] : items) {
    // Already created ;) skipping...
    if (m_Items.find(key) != m_Items.end())
      continue;
    auto item = CreateScope<BigMatrixItem>(key, m_id);
    addAndMakeVisible(*item);
    m_Items[key] = std::move(item);
  }
  UpdateSize();
}

} // namespace VeNo::GUI