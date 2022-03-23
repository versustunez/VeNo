#pragma once

#include <JuceHeader.h>

namespace VeNo::GUI {

class VComponent : public juce::Component {
public:
  template <class ClassType> ClassType *parent() {
    return findParentComponentOfClass<ClassType>();
  }
  template <class T>
  static Ref<T> create(size_t id) {
    return CreateRef<T>("", "", id);
  }

  template <class AsClass> AsClass *as() {
    return dynamic_cast<AsClass *>(this);
  }
};
} // namespace VeNo::GUI