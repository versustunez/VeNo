#pragma once

#include "FX.h"
#include "VeNo/Sound/Structs.h"
namespace VeNo::Audio {
class FXChain {
public:
  explicit FXChain(InstanceID id);
  void process(Channel &channel);

protected:
  InstanceID m_ID;
  Vector<Ref<FX>> m_FX;
};
} // namespace VeNo::Audio