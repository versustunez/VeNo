#pragma once

#include <JuceHeader.h>
#include <juce_dsp/juce_dsp.h>

namespace VeNo::Utils {
class FFT {
public:
  explicit FFT(int bits) : m_fft(bits) {}
  ~FFT();
  void prepare(float *data, size_t size);
  float *data();
  void forward();
  void reverse();

private:
  juce::dsp::FFT m_fft;
  float *m_internal = nullptr;
  size_t m_len;
};
} // namespace VeNo::Utils