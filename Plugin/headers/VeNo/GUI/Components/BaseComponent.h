#pragma once
#include <JuceHeader.h>
#include "../GuiLang/Structs.h"

namespace VeNo::GUI
{
enum LabelPosition
{
    NO_LABEL = 0,
    BOTTOM,
    TOP,
    CENTER
};

class BaseComponent : public juce::Component
{
public:
    BaseComponent (std::string name, std::string showName, size_t id);
    ~BaseComponent() override = default;

    std::string& name();
    std::string& showName();
    size_t id();
    LabelPosition m_labelPosition = LabelPosition::BOTTOM;
    Position pos{};

    void createLabel(const std::string& text, bool visible);
    void setText(const std::string& text);

    Position resize();
    void setLabelPosition(const std::string& pos);

protected:
    std::string m_name;
    std::string m_showName;
    std::unique_ptr<juce::Label> m_label;
    size_t m_id; // ID is the id to the Instance! it's provided by the GUI-Interpreter
};
} // namespace VeNo::GUI