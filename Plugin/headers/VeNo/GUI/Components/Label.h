#pragma once
#include <JuceHeader.h>
#include "BaseComponent.h"


namespace VeNo::GUI {
class Label : public BaseComponent {
public:
    Label (std::string name, std::string showName, size_t id);
    void setText(std::string text);
    void resized() override;

protected:
    std::string m_text;
    std::unique_ptr<juce::Label> m_label;
};
}