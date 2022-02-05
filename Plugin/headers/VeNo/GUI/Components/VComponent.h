#pragma once

#include <JuceHeader.h>

namespace VeNo::GUI {

class VComponent : public juce::Component {
public:
  template <class ClassType> ClassType *parent() {
    return findParentComponentOfClass<ClassType>();
  }
};
} // namespace VeNo::GUI