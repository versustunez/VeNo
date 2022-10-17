#pragma once

#include "VeNo/Sound/Structs.h"
namespace VeNo::Audio {
class FX {
public:
  explicit FX(InstanceID id) : m_ID(id) {}
  virtual ~FX() = default;
  virtual void update() = 0;
  virtual void process(Channel &) = 0;
  virtual void setSampleRate(double) {}
  VString &GetName() { return m_Name; }
  void SetName(const VString &name) { m_Name = name; }

protected:
  InstanceID m_ID;
  VString m_Name{"FX"};
};
} // namespace VeNo::Audio