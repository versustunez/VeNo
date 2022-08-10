#pragma once

#include "FX.h"
#include "VeNo/Sound/Structs.h"
namespace VeNo::Audio {
class FXChain {
public:
  explicit FXChain(InstanceID id);
  void process(Channel &channel);
  Vector<size_t>& GetSorted() { return m_SortedFX; }
  Vector<Ref<FX>>& GetFXData() { return m_FX; }
  void Deserialize(const VString& data);
  VString Serialize();
  void SetSortedList(Vector<size_t> newSortedData);

protected:
  void FixMissingFX();
protected:
  InstanceID m_ID;
  Vector<Ref<FX>> m_FX;
  Vector<size_t> m_SortedFX;
};
} // namespace VeNo::Audio