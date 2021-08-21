#include <VeNo/Core/Instance.h>
#include <VeNo/GUI/Components/ToggleButton.h>
namespace VeNo::GUI
{
ToggleButton::ToggleButton (const std::string& name, const std::string& showName, size_t id)
    : BaseComponent (name, showName, id)
{
    auto* instance = VeNo::Core::Instance::get (m_id);
    m_button = std::make_unique<juce::ToggleButton>();
    if (instance && instance->treeState)
    {
        auto state = instance->treeState;
        if (state->getParameter (m_name) != nullptr)
            m_attachment = std::make_unique<ButtonAttachment> (*state, m_name, *m_button);
    }
    addAndMakeVisible (*m_button);
}
void ToggleButton::setButtonText (const std::string& text)
{
    m_button->setButtonText (text);
}
ToggleButton::~ToggleButton()
{
    m_attachment.reset (nullptr);
    m_button.reset (nullptr);
}
void ToggleButton::resized()
{
    m_button->setBounds (0, 0, getWidth(), getHeight());
}
} // namespace VeNo::GUI
