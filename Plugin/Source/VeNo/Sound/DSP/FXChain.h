#pragma once

#include "FX.h"
#include "VeNo/Sound/Structs.h"
namespace VeNo::Audio {

class StepLowPass {
public:
  StepLowPass();
  float DoFilter(float input, float cut);
public:
  float output{0.0};
  int counter{0};
  float feedback[4][16]{};
  float filterOutput[4]{};
};
class FXChain {
public:
  explicit FXChain(InstanceID id);
  void process(Channel &channel);
  void SetSampleRate(double sampleRate);
  Vector<size_t> &GetSorted() { return m_SortedFX; }
  Vector<Ref<FX>> &GetFXData() { return m_FX; }
  void Deserialize(const VString &data);
  VString Serialize();
  void SetSortedList(Vector<size_t> newSortedData);

protected:
  void FixMissingFX();

protected:
  InstanceID m_ID;
  Vector<Ref<FX>> m_FX;
  Vector<size_t> m_SortedFX;
  StepLowPass m_PreFilter[2]{};
  StepLowPass m_PostFilter[2]{};
};
} // namespace VeNo::Audio