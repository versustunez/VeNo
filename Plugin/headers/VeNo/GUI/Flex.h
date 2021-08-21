#pragma once

#include <JuceHeader.h>
typedef juce::FlexBox FlexBox;
namespace VeNo::GUI
{
class ComponentGroup;
class BaseComponent;
class Flex
{
public:
    void setItems (std::vector<std::shared_ptr<BaseComponent>>&, std::vector<std::shared_ptr<ComponentGroup>>&);
    void perform (juce::Rectangle<float> rectangle);
    void clear();
    void setJustifyContent(std::string& value);
    void setWrap(std::string& value);
    void setAlignContent(std::string& value);
    void setAlignItems(std::string& value);
    void setDirection(std::string& value);
    float minWidth = 50.0f;
    float minHeight = 50.0f;

protected:
    FlexBox m_flexBox;
};
} // namespace VeNo::GUI