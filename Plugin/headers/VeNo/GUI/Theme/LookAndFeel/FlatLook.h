#pragma once

#include <JuceHeader.h>
#include <VeNo/GUI/Theme/Theme.h>

namespace VeNo::GUI
{
class FlatLook : public juce::LookAndFeel_V4
{
private:
public:
    FlatLook();

    ~FlatLook() override;

    void drawButtonBackground (juce::Graphics& graphics, juce::Button& button, const juce::Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    void drawRotarySlider (juce::Graphics& graphics, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;

    void drawTextEditorOutline (juce::Graphics& graphics, int width, int height, juce::TextEditor& editor) override;

    void drawToggleButton (juce::Graphics& graphics, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    void drawTabButton (juce::TabBarButton& button, juce::Graphics& graphics, bool isMouseOver, bool isMouseDown) override;

    void
        drawComboBox (juce::Graphics& graphics, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox& box) override;

    void drawLabel (juce::Graphics& graphics, juce::Label& label) override;

protected:
    VeNo::Theme::Theme* theme = nullptr;
};
} // namespace VeNo::Look
