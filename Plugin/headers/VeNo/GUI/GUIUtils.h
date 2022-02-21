#pragma once

#include <JuceHeader.h>
#include <VeNo/Core/Instance.h>

namespace VeNo::GUI {
class Utils {
public:
  static int getScaledSize(int size);
  static float getScaledSize(float size);
  static double setFontSize(double size, juce::Graphics &g);
  static juce::Path drawRectangleWithRadius(int x, int y, int w, int h,
                                            float radius, juce::Graphics &g);
  static VeNo::Core::Instance *
  getInstanceOrNull(const juce::Component &component);

  template <class T> static T *instanceOf(juce::Component *component) {
    return dynamic_cast<T *>(component);
  }
  template <class Base,class T> static T *instanceOf(Base *instance) {
    return dynamic_cast<T *>(instance);
  }
};
} // namespace VeNo::GUI