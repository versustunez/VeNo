#pragma once

#include "Modulatoren.h"

#include <VeNo/Core/Parameter/Parameter.h>
#include <VeNo/TypeDefs.h>

namespace VeNo::Audio {
struct ModulationItem {
  Modulator *src{nullptr};
  Core::ModulateParameter *dst{nullptr};
};
class Matrix {
public:
  explicit Matrix(InstanceID instance_id);
  ~Matrix();
  void update();
  void addModulator(const Ref<Modulator> &modulator) {
    m_modulators.push_back(modulator);
  }
  void remove(const VString& name, const VString& dst);
  ModulatorHandle& handle() { return m_modulatorHandle; }

protected:
  InstanceID m_id{0};
  Vector<Ref<Modulator>> m_modulators{};
  Vector<ModulationItem> m_items{};
  Map<VString, size_t> m_indexLookup{};
  Vector<size_t> m_deletedItems;
  ModulatorHandle m_modulatorHandle;
};
} // namespace VeNo::Audio