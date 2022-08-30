#pragma once

#include "Modulatoren.h"

#include <VeNo/Core/Parameter/Parameter.h>
#include <VeNo/TypeDefs.h>

namespace VeNo::Audio {
struct ModulationItem {
  Modulator *Source{nullptr};
  Core::ModulateParameter *Destination{nullptr};
  double Amount{0.0};
};
class Matrix {
public:
  explicit Matrix(InstanceID instance_id);
  ~Matrix();
  void update();
  void addModulator(const Ref<Modulator> &modulator) {
    m_modulators.push_back(modulator);
  }
  void remove(const VString &key);
  bool add(const VString &modulator, const VString &dst);
  void setAmount(const VString &key, double amount);
  bool Has(const VString& key) { return m_items.find(key) != m_items.end(); }
  double GetAmount(const VString &key) { return m_items[key].Amount; }
  ModulatorHandle &handle() { return m_modulatorHandle; }
  const Vector<Ref<Modulator>> &Modulators() { return m_modulators; }

  juce::XmlElement *ToXml();
  void FromXML(const Scope<juce::XmlElement> &data);

protected:
  InstanceID m_id{0};
  Vector<Ref<Modulator>> m_modulators{};
  Map<VString, ModulationItem> m_items{};
  ModulatorHandle m_modulatorHandle;
  juce::CriticalSection m_Mutex;
};
} // namespace VeNo::Audio