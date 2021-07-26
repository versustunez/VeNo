#include <VeNo/Core/Config.h>
#include <VeNo/GUI/GUIUtils.h>
#include <VeNo/GUI/Theme/LookAndFeel/LookHandler.h>

namespace VeNo::GUI
{
typedef VeNo::Theme::Colors Colors;
LookHandler::LookHandler()
{
    theme = VeNo::Core::Config::get().theme().get();
    setDefaultLookAndFeel (this);
}

LookHandler::~LookHandler()
{
    DBGN ("Delete LookHandler")
    delete m_feels[0];
    m_feels[0] = nullptr;
    setDefaultLookAndFeel(nullptr);
}

void LookHandler::selectLook (int index)
{
    m_currentLook = index;
}

juce::LookAndFeel_V4* LookHandler::getLook()
{
    assert (m_feels[0] != nullptr);
    return m_feels[m_currentLook];
}

void LookHandler::drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    getLook()->drawButtonBackground (g, button, backgroundColour, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
}

void LookHandler::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    getLook()->drawRotarySlider (g, x, y, width, height, sliderPosProportional, rotaryStartAngle, rotaryEndAngle, slider);
}

void LookHandler::drawTextEditorOutline (juce::Graphics& graphics, int width, int height, juce::TextEditor& editor)
{
    getLook()->drawTextEditorOutline (graphics, width, height, editor);
}

void LookHandler::drawToggleButton (juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    getLook()->drawToggleButton (g, button, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
}

void LookHandler::drawTabButton (juce::TabBarButton& btn, juce::Graphics& g, bool isMouseOver, bool isMouseDown)
{
    getLook()->drawTabButton (btn, g, isMouseOver, isMouseDown);
}

void LookHandler::drawComboBox (juce::Graphics& g, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox& box)
{
    getLook()->drawComboBox (g, width, height, isButtonDown, buttonX, buttonY, buttonW, buttonH, box);
}

void LookHandler::drawLabel (juce::Graphics& graphics, juce::Label& label)
{
    getLook()->drawLabel (graphics, label);
}

void LookHandler::drawTabAreaBehindFrontButton (juce::TabbedButtonBar&, juce::Graphics& g, int w, int h)
{
    g.setColour (theme->getDirectColor (Colors::bg_two).withAlpha (0.5f));
    g.drawLine (0, h, w, h);
}

juce::PopupMenu::Options LookHandler::getOptionsForComboBoxPopupMenu (juce::ComboBox& box, juce::Label&)
{
    return juce::PopupMenu::Options()
        .withTargetComponent (&box)
        .withItemThatMustBeVisible (box.getSelectedId())
        .withMinimumWidth (box.getWidth())
        .withMaximumNumColumns (3)
        .withStandardItemHeight (Utils::getScaledSize (25));
}

void LookHandler::drawPopupMenuItem (juce::Graphics& g, const juce::Rectangle<int>& area, bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu, const juce::String& text, const juce::String&, const juce::Drawable*, const juce::Colour*)
{
    if (isSeparator)
    {
        auto r = area.reduced (5, 0);
        r.removeFromTop (juce::roundToInt (((float) r.getHeight() * 0.5f) - 0.5f));

        g.setColour (findColour (juce::PopupMenu::textColourId).withAlpha (0.3f));
        g.fillRect (r.removeFromTop (1));
    }
    else
    {
        auto textColour = theme->getDirectColor (Colors::font);
        auto r = area.reduced (1);
        if (isHighlighted && isActive)
        {
            g.setColour (theme->getDirectColor (Colors::accent).withAlpha (0.4f));
            g.fillRect (r);
            g.setColour (theme->getDirectColor (Colors::font));
        }
        else
        {
            g.setColour (textColour.withMultipliedAlpha (isActive ? 1.0f : 0.5f));
        }
        auto font = getPopupMenuFont();
        auto maxFontHeight = (float) r.getHeight() / 1.3f;
        if (font.getHeight() > maxFontHeight)
            font.setHeight (maxFontHeight);

        g.setFont (font);
        if (hasSubMenu)
        {
            auto arrowH = 0.6f * getPopupMenuFont().getAscent();

            auto x = static_cast<float> (r.removeFromRight ((int) arrowH).getX());
            auto halfH = static_cast<float> (r.getCentreY());

            juce::Path path;
            path.startNewSubPath (x, halfH - arrowH * 0.5f);
            path.lineTo (x + arrowH * 0.6f, halfH);
            path.lineTo (x, halfH + arrowH * 0.5f);

            g.strokePath (path, juce::PathStrokeType (2.0f));
        }

        r.removeFromRight (3);
        r.removeFromLeft (10);
        g.drawFittedText (text, r, hasSubMenu ? juce::Justification::centred : juce::Justification::centredLeft, 1);

        if (isTicked)
        {
            g.setColour (theme->getDirectColor (Colors::accent));
            g.fillRect (0, 0, 5, r.getHeight());
        }
    }
}

void LookHandler::drawPopupMenuBackground (juce::Graphics& g, int width, int height)
{
    g.fillAll (theme->getDirectColor (Colors::bg));
    juce::ignoreUnused (width, height);

#if ! JUCE_MAC
    g.setColour (theme->getDirectColor (Colors::bg).withAlpha (0.2f));
    g.drawRect (0, 0, width, height);
#endif
}
} // namespace VeNo::GUI