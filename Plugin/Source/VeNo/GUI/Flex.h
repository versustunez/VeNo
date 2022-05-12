#pragma once

#include <JuceHeader.h>
typedef juce::FlexBox FlexBox;
namespace VeNo::GUI {
class ComponentGroup;
class BaseComponent;
class Flex {
public:
  Flex() = default;
  explicit Flex(std::unordered_map<std::string, std::string> &properties);
  void setItems(std::vector<Ref<BaseComponent>> &,
                std::vector<Ref<ComponentGroup>> &);
  void perform(juce::Rectangle<float> rectangle);
  void clear();
  void setJustifyContent(std::string &value);
  void setWrap(std::string &value);
  void setAlignContent(std::string &value);
  void setAlignItems(std::string &value);
  void setDirection(std::string &value);
  float minWidth = 50.0f;
  float minHeight = 50.0f;

  FlexBox* raw() { return &m_flexBox; }

protected:
  FlexBox m_flexBox;
};
} // namespace VeNo::GUI