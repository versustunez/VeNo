#pragma once
#include "../GuiLang/Structs.h"
#include <JuceHeader.h>
#include <VeNo/Events/EventHandler.h>

namespace VeNo::GUI
{
enum LabelPosition
{
    NO_LABEL = 0,
    BOTTOM,
    TOP,
    CENTER
};

class Interpreter;

class BaseComponent : public juce::Component
{
public:
    BaseComponent (std::string name, std::string showName, size_t id);
    ~BaseComponent() override = default;

    void setSelectorId(std::string selector);
    std::string& name();
    std::string& showName();
    size_t id() const;
    std::string& selectorId();
    LabelPosition m_labelPosition = LabelPosition::BOTTOM;
    Position pos {};

    void createLabel (const std::string& text, bool visible);
    void setText (const std::string& text);
    void triggerAfterParsing (Interpreter*);

    Position resize();
    void setLabelPosition (const std::string& pos);
    void setEventHandler(Events::EventHandler*);

protected:
    virtual void afterParsing (Interpreter*);
    std::string m_name;
    std::string m_showName;
    std::unique_ptr<juce::Label> m_label;
    size_t m_id; // ID is the id to the Instance! it's provided by the GUI-Interpreter
    std::string m_selectorId;
    bool m_afterParsingCalled = false;
    Events::EventHandler* m_handler {};
};
} // namespace VeNo::GUI