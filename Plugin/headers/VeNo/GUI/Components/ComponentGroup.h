#pragma once

#include "BaseComponent.h"
#include <JuceHeader.h>
#include <VUtils/Logging.h>
#include <VeNo/GUI/GuiLang/Structs.h>
#include <memory>

namespace VeNo::GUI
{
struct ComponentGroup : public juce::Component
{
    ~ComponentGroup() override;
    // Components can have Sub-Groups
    std::vector<std::shared_ptr<ComponentGroup>> groups;
    // List of all Components of the current groups
    std::vector<std::shared_ptr<BaseComponent>> components;
    void paint (juce::Graphics& g) override;
    void resized() override;
    void showChilds();
    void setColor(const GUIColorComponent& color);
    void setPosition(Position pos);

protected:
    GUIColorComponent m_color;
    Position m_pos;
};
} // namespace VeNo::GUI