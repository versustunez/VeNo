#include <VUtils/Logging.h>
#include <VeNo/GUI/Components/BaseComponent.h>
#include <VeNo/GUI/GUIUtils.h>
namespace VeNo::GUI
{
BaseComponent::BaseComponent (std::string name, std::string showName, size_t id)
    : m_name (std::move (name)), m_showName (std::move (showName)), m_id (id)
{
}
std::string& BaseComponent::name()
{
    return m_name;
}
std::string& BaseComponent::showName()
{
    return m_showName;
}

size_t BaseComponent::id()
{
    return m_id;
}
void BaseComponent::createLabel (const std::string& text, bool visible)
{
    m_label = std::make_unique<juce::Label>();
    setText (text);
    if (visible)
        addAndMakeVisible (*m_label);
}

void BaseComponent::setText (const std::string& text)
{
    if (m_label != nullptr)
        m_label->setText (text, juce::NotificationType::dontSendNotification);
}
Position BaseComponent::resize()
{
    if (m_label != nullptr)
    {
        int height = Utils::getScaledSize (16);
        int secHeight = getHeight() - height;
        LabelPosition position = m_labelPosition;
        if (position == LabelPosition::TOP)
        {
            m_label->setBounds (0, 0, getWidth(), height);
            return { 0, height, getWidth(), secHeight };
        }
        else if (position == LabelPosition::BOTTOM)
        {
            m_label->setBounds (0, secHeight, getWidth(), height);
            return { 0, 0, getWidth(), secHeight };
        }
        else if (position == LabelPosition::CENTER)
        {
            int halfHeight = (getHeight() - height) / 2;
            m_label->setBounds (0, halfHeight, getWidth(), height);
            return { 0, 0, getWidth(), getHeight() };
        }
    }
    return { 0, 0, getWidth(), getHeight() };
}
void BaseComponent::setLabelPosition (const std::string& label)
{
    if (label == "no")
        m_labelPosition = LabelPosition::NO_LABEL;
    else if (label == "bottom")
        m_labelPosition = LabelPosition::BOTTOM;
    else if (label == "top")
        m_labelPosition = LabelPosition::TOP;
    else if (label == "center")
        m_labelPosition = LabelPosition::CENTER;
    else
    {
        WARN ("Label Position %s is unknown available: no,bottom,top,center", label.c_str())
    }
}
} // namespace VeNo::GUI
