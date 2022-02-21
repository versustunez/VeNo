#include <VeNo/Utils/FFT.h>

namespace VeNo::Utils {
void FFT::prepare(float *data, size_t size) {
  delete[] m_internal;
  size_t intSize = size * 2;
  m_internal = new float[intSize];
  std::memset(m_internal, 0, intSize * sizeof(float));
  m_len = size;
  std::memcpy(m_internal, data, size * sizeof(float));
}

float *FFT::data() {
  auto output = new float[m_len];
  std::memcpy(output, m_internal, m_len * sizeof(float));
  return output;
}
void FFT::forward() { m_fft.performRealOnlyForwardTransform(m_internal, true); }
void FFT::reverse() { m_fft.performRealOnlyInverseTransform(m_internal); }
FFT::~FFT() { delete[] m_internal; }
} // namespace VeNo::Utils