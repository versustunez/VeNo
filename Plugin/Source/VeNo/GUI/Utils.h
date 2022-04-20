#pragma once
#include <JuceHeader.h>

namespace VeNo::GUI {
class Utils {
public:
  template <class T> static T *instanceOf(juce::Component *component) {
    return dynamic_cast<T *>(component);
  }
  template <class Base,class T> static T *instanceOf(Base *instance) {
    return dynamic_cast<T *>(instance);
  }
};

}