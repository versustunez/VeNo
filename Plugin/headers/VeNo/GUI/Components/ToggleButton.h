#pragma once
#include "BaseComponent.h"
typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
namespace VeNo::GUI
{
class ToggleButton : public BaseComponent {
public:
    ToggleButton (const std::string& name, const std::string& showName, size_t id);
    ~ToggleButton() override;
    void setButtonText(const std::string&);
    void resized() override;
private:
    std::unique_ptr<juce::ToggleButton> m_button;
    std::unique_ptr<ButtonAttachment> m_attachment;
};
}