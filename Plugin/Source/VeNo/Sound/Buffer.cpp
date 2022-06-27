#include "Buffer.h"

#include "VeNo/Core/Instance.h"

namespace VeNo::Audio {
Buffer::Buffer(VeNo::InstanceID id) : m_ID(id) {
    m_Handler = &Core::Instance::get(m_ID)->eventHandler;
}
void Buffer::reset(int size) {
  size = size < 2048 ? 2048 : size;
  m_Right.resize((size_t)size);
  m_Left.resize((size_t)size);
}

// rebuild to be a FIFO-Buffer :>
void Buffer::addSamples(const juce::AudioBuffer<float> &buffer) {
  auto size = (size_t)buffer.getNumSamples();
  if (size < m_Left.size()) {
    size_t until = m_Left.size() - size;
    for (size_t i = size; i < m_Left.size(); ++i) {
      m_Left[i-size] = m_Left[i];
      m_Right[i-size] = m_Right[i];
    }
    for (size_t i = 0; i < size; ++i) {
      m_Left[i + until] = buffer.getSample(0, (int)i);
      m_Right[i + until] = buffer.getSample(1, (int)i);
    }
  } else {
    for (size_t i = 0; i < size; ++i) {
      m_Left[i] = buffer.getSample(0,(int)i);
      m_Right[i] = buffer.getSample(1,(int)i);
    }
  }
  m_Handler->triggerEvent("lcd-change", new Events::TriggerEvent);
}
};