#include <VeNo/GUI/Components/Label.h>

#include <utility>

namespace VeNo::GUI
{
Label::Label (std::string name, std::string showName, size_t id)
    : BaseComponent (std::move (name), std::move (showName), id)
{
    m_label = std::make_unique<juce::Label>();
    addAndMakeVisible (*m_label);
}
void Label::setText (std::string text)
{
    m_text = std::move (text);
    m_label->setText (m_text, juce::NotificationType::dontSendNotification);
}
void Label::resized()
{
    m_label->setBounds (0, 0, getWidth(), getHeight());
}
} // namespace VeNo::GUI