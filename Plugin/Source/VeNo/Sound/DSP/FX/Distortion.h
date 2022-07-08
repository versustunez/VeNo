#pragma once

#include "VeNo/Sound/DSP/FX.h"
namespace VeNo::Audio {
class Distortion : public FX {
public:
  explicit Distortion(InstanceID id);
  void update() override;
  void process(Channel &channel) override;
};
}