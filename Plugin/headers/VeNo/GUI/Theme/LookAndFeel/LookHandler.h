#pragma once

#include "FlatLook.h"
#include <JuceHeader.h>
#include <memory>

namespace VeNo::GUI
{
class LookHandler : public juce::LookAndFeel_V4, juce::DeletedAtShutdown
{
private:
    int m_currentLook = 0;

public:
    LookHandler();

    ~LookHandler() override;
    void selectLook (int index);

    LookAndFeel_V4* getLook();

    void drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;

    void drawTextEditorOutline (juce::Graphics& graphics, int width, int height, juce::TextEditor& editor) override;

    void drawToggleButton (juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    void drawTabButton (juce::TabBarButton& btn, juce::Graphics& g, bool isMouseOver, bool isMouseDown) override;

    void drawComboBox (juce::Graphics& g, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox& box) override;

    void drawLabel (juce::Graphics& graphics, juce::Label& label) override;

    void drawTabAreaBehindFrontButton (juce::TabbedButtonBar& bar, juce::Graphics& g, int w, int h) override;

    void drawPopupMenuItem (juce::Graphics& graphics, const juce::Rectangle<int>& area, bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu, const juce::String& text, const juce::String& shortcutKeyText, const juce::Drawable* icon, const juce::Colour* textColour) override;

    juce::PopupMenu::Options getOptionsForComboBoxPopupMenu (juce::ComboBox& box, juce::Label& label) override;

    void drawPopupMenuBackground (juce::Graphics& graphics, int width, int height) override;

protected:
    //currently both available themes are CrazyLook <-- (this is a fun one xD) and FlatLook
    LookAndFeel_V4* m_feels[1] = { new FlatLook() };
    VeNo::Theme::Theme* theme = nullptr;
};

} // namespace VeNo::GUI
