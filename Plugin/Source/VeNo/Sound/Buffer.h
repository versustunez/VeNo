#pragma once
#include "VeNo/Events/EventHandler.h"

#include <JuceHeader.h>
#include <VeNo/TypeDefs.h>

namespace VeNo::Audio {
class Buffer {
public:
  explicit Buffer(InstanceID id);
  void reset(int size);
  void addSamples(const juce::AudioBuffer<float>& buffer);
  // COPY!
  Vector<double> getChannel(int channel) {
    return channel == 0 ? m_Left : m_Right;
  }

protected:
  InstanceID m_ID{};
  Vector<double> m_Left{};
  Vector<double> m_Right{};
  Events::EventHandler* m_Handler{nullptr};
};
} // namespace VeNo::Sound