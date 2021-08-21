#include <VUtils/Math.h>
#include <VeNo/Core/Config.h>
#include <VeNo/GUI/Fonts/Fonts.h>
#include <VeNo/GUI/GUIUtils.h>
#include <VeNo/GUI/Theme/LookAndFeel/FlatLook.h>
#include <VeNo/GUI/Theme/Theme.h>

namespace VeNo::GUI
{
typedef VeNo::Theme::Colors Colors;
FlatLook::FlatLook()
{
    setDefaultSansSerifTypeface (VeNo::GUI::Fonts::getNormal()->getTypeface());
    theme = VeNo::Core::Config::get().theme().get();
}

FlatLook::~FlatLook()
{
    setDefaultSansSerifTypeface(nullptr);
    theme = nullptr;
}

void FlatLook::drawButtonBackground (juce::Graphics& graphics, juce::Button& button, const juce::Colour&, bool shouldDrawButtonAsHighlighted, bool)
{
    auto buttonArea = button.getLocalBounds();
    if (shouldDrawButtonAsHighlighted)
        graphics.setColour (theme->getDirectColor (VeNo::Theme::Colors::accent));
    else
        graphics.setColour (theme->getDirectColor (VeNo::Theme::Colors::accent_two));
    graphics.drawRect (buttonArea);
}

void FlatLook::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    float MAX_RADIAN = 2.53073;
    float radius = juce::jmin (width / 2.0f, height / 2.0f) -3.5f;
    auto centreX = x + width * 0.5f;
    auto centreY = y + height * 0.5f;
    auto key = slider.getName().toStdString();
    float arcPos = sliderPosProportional;
    auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    auto instance = Utils::getInstanceOrNull (slider);
    if (instance)
    {
        auto param = instance->handler->getModulateParameter (key);
        if (param != nullptr)
            arcPos = (float) param->getVoice (0);
    }
    float size = 3.5f;

    //---[the real draw]---//
    juce::Path outerArc;
    outerArc.addCentredArc (0, 0, radius, radius, 0, -MAX_RADIAN, MAX_RADIAN, true);
    outerArc.applyTransform (juce::AffineTransform().translated (centreX, centreY));
    g.setColour (theme->getDirectColor (Colors::accent).withAlpha (0.2f));
    g.strokePath (outerArc, juce::PathStrokeType (size));

    //prepare pointer for drawing
    juce::Path arc;
    arc.addCentredArc (0, 0, radius, radius, 0, -MAX_RADIAN, (float) VUtils::Math::clamp (arcPos * 2 - 1, -1, 1) * MAX_RADIAN, true);
    arc.applyTransform (juce::AffineTransform().translated (centreX, centreY));
    auto colourTwo = theme->getDirectColor (Colors::accent_two);
    auto colourMain = theme->getDirectColor (Colors::accent);
    g.setGradientFill (juce::ColourGradient::horizontal (colourTwo, centreX - radius, colourMain, centreX + radius));
    g.strokePath (arc, juce::PathStrokeType (size));

    juce::Path pointer;
    auto pointerThickness = 3;
    auto l = 5;
    pointer.addEllipse (-pointerThickness * 0.25f, -(radius - l), pointerThickness, pointerThickness);
    pointer.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));
    g.setColour (theme->getDirectColor (Colors::accent));
    g.fillPath (pointer);
}

void FlatLook::drawTextEditorOutline (juce::Graphics& graphics, int width, int height, juce::TextEditor&)
{
    graphics.setGradientFill (
        juce::ColourGradient::horizontal (
            theme->getDirectColor (Colors::accent_two),
            0,
            theme->getDirectColor (Colors::accent),
            (float)width));
    auto p = 10.0f;
    graphics.drawLine (p, (float)height, (float)width - p, (float)height, 0.9f);
}

void FlatLook::drawToggleButton (juce::Graphics& graphics, juce::ToggleButton& button, bool, bool shouldDrawButtonAsDown)
{
    int height = button.getHeight();
    int width = button.getWidth();
    auto text = button.getButtonText();
    if (shouldDrawButtonAsDown || button.getToggleStateValue() == button.buttonDown)
    {
        graphics.setColour (theme->getDirectColor (Colors::accent));
        if (text == "")
            text = "On";
    }
    else
    {
        graphics.setColour (theme->getDirectColor (Colors::accent).withAlpha (0.2f));
        if (text == "")
            text = "Off";
    }

    auto l = 2;
    auto p = 4;
    graphics.fillRect (l, l, width - p, height - p);
    graphics.setColour (theme->getDirectColor (Colors::font));
    graphics.drawText (text, l, l, width - p, height - p, juce::Justification::centred, true);
}

void FlatLook::drawTabButton (juce::TabBarButton& button, juce::Graphics& graphics, bool, bool)
{
    const juce::Rectangle<int> activeArea (button.getActiveArea());
    activeArea.withHeight (activeArea.getHeight() - 1);
    graphics.setColour (theme->getDirectColor (Colors::bg));
    graphics.fillRect (activeArea);

    int width = button.getWidth();
    int height = button.getHeight() - 1;
    if (button.getToggleState())
        graphics.setColour (theme->getDirectColor (Colors::accent));
    else
        graphics.setColour (theme->getDirectColor (Colors::accent).withAlpha (0.2f));
    auto p = 2;
    graphics.fillRect (0, height - p, width, p);
    graphics.setColour (theme->getDirectColor (Colors::font));
    graphics.drawText (button.getButtonText(), 0, 0, width, height, juce::Justification::centred, true);
}

void FlatLook::drawComboBox (juce::Graphics& graphics, int width, int height, bool, int, int, int, int, juce::ComboBox&)
{
    auto borderHeight = 2;
    graphics.setColour (theme->getDirectColor (Colors::accent));
    graphics.fillRect (0, height - borderHeight, width, borderHeight);
    graphics.setColour (theme->getDirectColor (Colors::bg_two).withAlpha (0.2f));
    graphics.fillRect (0, 0, width, height - borderHeight);
}

void FlatLook::drawLabel (juce::Graphics& graphics, juce::Label& label)
{
    if (! label.isBeingEdited())
    {
        auto alpha = label.isEnabled() ? 1.0f : 0.5f;
        const juce::Font font (getLabelFont (label));
        graphics.setColour (theme->getDirectColor (VeNo::Theme::Colors::font).withAlpha (alpha));
        auto textArea = getLabelBorderSize (label).subtractedFrom (label.getLocalBounds());
        graphics.drawFittedText (label.getText(), textArea, label.getJustificationType(), juce::jmax (1, (int) ((float) textArea.getHeight() / font.getHeight())), label.getMinimumHorizontalScale());
    }
}
} // namespace VeNo::GUI
