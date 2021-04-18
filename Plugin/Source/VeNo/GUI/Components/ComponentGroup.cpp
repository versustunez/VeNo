#include <VeNo/GUI/Components/ComponentGroup.h>

#include <VeNo/Core/Config.h>
#include <VeNo/GUI/GUIUtils.h>

namespace VeNo::GUI
{
ComponentGroup::~ComponentGroup() = default;
void ComponentGroup::paint (juce::Graphics& g)
{
    if (! m_color.hasColor)
        return;
    if (m_color.isPreColor)
    {
        g.setColour (VeNo::Core::Config::get().theme()->getColor (m_color.preColor));
    }
    else
    {
        auto& colors = m_color.colors;
        g.setColour (juce::Colour (colors[0], colors[1], colors[2], colors[3]));
    }
    g.fillAll();
}
void ComponentGroup::resized()
{
    for (auto& subGroup : groups)
        subGroup->setBounds (
            Utils::getScaledSize (subGroup->m_pos.x),
            Utils::getScaledSize (subGroup->m_pos.y),
            Utils::getScaledSize (subGroup->m_pos.w),
            Utils::getScaledSize (subGroup->m_pos.h));
    for (auto& child : components)
        child->setBounds (
            Utils::getScaledSize (child->pos.x),
            Utils::getScaledSize (child->pos.y),
            Utils::getScaledSize (child->pos.w),
            Utils::getScaledSize (child->pos.h));
}
void ComponentGroup::showChilds()
{
    for (auto& subGroup : groups)
    {
        addAndMakeVisible (*subGroup);
        subGroup->showChilds();
    }
    for (auto& child : components)
        addAndMakeVisible (child.get());
}
void ComponentGroup::setColor (const GUIColorComponent& color)
{
    m_color = color;
}
void ComponentGroup::setPosition (Position pos)
{
    m_pos = pos;
}
} // namespace VeNo::GUI