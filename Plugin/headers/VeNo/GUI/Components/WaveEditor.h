#pragma once
#include <VeNo/GUI/Components/BaseComponent.h>

namespace VeNo::GUI {

class WaveEditor : public BaseComponent {
public:
    WaveEditor (std::string, const std::string&, size_t);
    void paint (juce::Graphics&) override;
    void resized() override;
protected:
    void afterParsing (Interpreter*) override;
};
}