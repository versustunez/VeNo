#pragma once

#include "VeNo/Sound/Structs.h"
namespace VeNo::Audio {
class FX {
public:
  explicit FX(InstanceID id) : m_ID(id){};
  virtual void update() = 0;
  virtual void process(Channel &) = 0;

protected:
  InstanceID m_ID;
};
} // namespace VeNo::Audio