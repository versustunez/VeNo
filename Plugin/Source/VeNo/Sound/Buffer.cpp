#include "Buffer.h"

#include "VeNo/Core/Instance.h"

namespace VeNo::Audio {
Buffer::Buffer(VeNo::InstanceID id) : m_ID(id) {
    m_Handler = &Core::Instance::get(m_ID)->eventHandler;
}
void Buffer::reset(int size) {
  m_Right.resize(size);
  m_Left.resize(size);
}

void Buffer::addSamples(const juce::AudioBuffer<float> &buffer) {
  for (int i = 0; i < buffer.getNumSamples(); ++i) {
    m_Left[i] = buffer.getSample(0,i);
    m_Right[i] = buffer.getSample(1,i);
  }
  m_Handler->triggerEvent("lcd-change", new Events::TriggerEvent);
}
};