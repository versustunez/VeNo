#include "VeNo/Core/Instance.h"

#include <VeNo/Sound/Synth/Matrix.h>

namespace VeNo::Audio {

Matrix::Matrix(InstanceID instance_id)
    : m_id(instance_id),
      m_modulatorHandle(instance_id) {
  m_modulatorHandle.init(this);
}
Matrix::~Matrix() {
  m_modulators.clear();
  m_items.clear();
}
void Matrix::update() {
  // Matrix is empty... don't update anything because we would waste cycles
  if (m_items.empty())
    return;

  for (auto &modulator : m_modulators) {
    modulator->update();
  }
  const juce::GenericScopedLock<juce::CriticalSection> myScopedLock(m_Mutex);
  for (auto &[key, item] : m_items) {
    item.Destination->Begin();
  }
  for (auto &[key, item] : m_items) {
    if (item.Source->isVoiceModulator()) {
      for (int i = 0; i < MAX_VOICES; ++i) {
        item.Destination->addValueVoice(i, item.Source->value(i) * item.Amount);
      }
    } else {
      item.Destination->addValue(item.Source->value(-1) * item.Amount);
    }
  }
  for (auto &[key, item] : m_items) {
    item.Destination->Finish();
  }
}
void Matrix::remove(const VString &key) {
  auto it = m_items.find(key);
  if (it == m_items.end())
    return;
  const juce::GenericScopedLock<juce::CriticalSection> myScopedLock(m_Mutex);
  m_items.erase(key);
}

void Matrix::reset() {
  const juce::GenericScopedLock<juce::CriticalSection> myScopedLock(m_Mutex);
  m_items.clear();
}

bool Matrix::add(const VString &modulatorKey, const VString &dst) {
  std::string key = modulatorKey + dst;
  auto it = m_items.find(key);
  if (it != m_items.end())
    return false;
  auto srcMod = std::find_if(m_modulators.begin(), m_modulators.end(),
                             [&](Ref<Modulator> &modulator) {
                               return modulator->name() == modulatorKey;
                             });
  if (srcMod == m_modulators.end())
    return false;
  auto *handler = Core::Instance::get(m_id)->handler.get();
  auto *parameter = handler->getModulateParameter(dst);
  // Parameter does not exist, skip adding it
  if (!parameter)
    return false;
  m_items[key] = {srcMod->get(), parameter, 0.0};
  return true;
}
void Matrix::setAmount(const VeNo::VString &key, double amount) {
  const juce::GenericScopedLock<juce::CriticalSection> myScopedLock(m_Mutex);
  auto it = m_items.find(key);
  if (it == m_items.end())
    return;
  it->second.Amount = amount;
}
juce::XmlElement *Matrix::ToXml() {
  auto matrix = new juce::XmlElement("Matrix");
  for (auto &[key, item] : m_items) {
    auto element = new juce::XmlElement("Matrix-Item");
    element->setAttribute("Source", item.Source->name());
    element->setAttribute("Destination", item.Destination->getName());
    element->setAttribute("Amount", item.Amount);
    matrix->addChildElement(element);
  }
  return matrix;
}
void Matrix::FromXML(const Scope<juce::XmlElement> &data) {
  const juce::GenericScopedLock<juce::CriticalSection> myScopedLock(m_Mutex);
  m_items.clear();
  auto *matrix = data->getChildByName("Matrix");
  if (!matrix)
    return;
  for (int i = 0; i < matrix->getNumChildElements(); ++i) {
    auto item = matrix->getChildElement(i);
    auto Source = item->getStringAttribute("Source").toStdString();
    auto Destination = item->getStringAttribute("Destination").toStdString();
    auto amount = item->getDoubleAttribute("Amount");
    add(Source, Destination);
    setAmount(Source + Destination, amount);
  }
}
} // namespace VeNo::Audio