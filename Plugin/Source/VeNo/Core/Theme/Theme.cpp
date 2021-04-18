#include <VeNo/Core/Theme/Theme.h>

#include <utility>

namespace VeNo::Theme
{
Theme::Theme (std::shared_ptr<juce::PropertiesFile> file)
    : m_configFile (std::move (file))
{
}
void Theme::init()
{
    getColourFromConfig (Colors::bg);
    getColourFromConfig (Colors::bg_two);
    getColourFromConfig (Colors::accent);
    getColourFromConfig (Colors::accent_two);
    getColourFromConfig (Colors::font);
    getColourFromConfig (Colors::clip);
    getColourFromConfig (Colors::lcd_bg);
    getColourFromConfig (Colors::lcd);

    m_colorMapping["primaryBG"] = VeNo::Theme::Colors::bg;
    m_colorMapping["secondaryBG"] = VeNo::Theme::Colors::bg_two;
    m_colorMapping["primary"] = VeNo::Theme::Colors::accent;
    m_colorMapping["secondary"] = VeNo::Theme::Colors::accent_two;
    m_colorMapping["font"] = VeNo::Theme::Colors::font;
    m_colorMapping["clip"] = VeNo::Theme::Colors::clip;
    m_colorMapping["lcdBG"] = VeNo::Theme::Colors::lcd_bg;
    m_colorMapping["lcd"] = VeNo::Theme::Colors::lcd;
}
juce::Colour Theme::getDirectColor (Colors index)
{
    if (m_colours[index] != nullptr)
        return *m_colours[index];
    return juce::Colour (255, 255, 255);
}
juce::Colour Theme::getColor (const std::string& name)
{
    return getDirectColor (m_colorMapping[name]);
}
void Theme::setColor (Colors index, juce::Colour* colour)
{
    delete m_colours[index];
    m_colours[index] = colour;
}
void Theme::getColourFromConfig (Colors index)
{
    std::string key = colorToString (index);
    delete m_colours[index];
    if (m_configFile->containsKey (key))
    {
        auto baseColour = juce::Colour::fromString (m_configFile->getValue (key));
        auto* colour = new juce::Colour (baseColour);
        m_colours[index] = colour;
    }
    else
    {
        m_colours[index] = new juce::Colour (getDefault (index));
    }
}
std::string Theme::colorToString (Colors index)
{
    switch (index)
    {
        case Colors::bg:
            return "primaryBG_color";
        case Colors::bg_two:
            return "secondaryBG_color";
        case Colors::accent:
            return "primary_color";
        case Colors::accent_two:
            return "secondary_color";
        case Colors::clip:
            return "clip_color";
        case Colors::font:
            return "font_color";
        case Colors::lcd_bg:
            return "lcdBG_color";
        case Colors::lcd:
            return "lcd_color";
        case Colors::unknown:
        default:
            return "";
    }
}
juce::Colour Theme::getDefault (Colors index)
{
    switch (index)
    {
        case Colors::bg:
            return juce::Colour (27, 27, 33);
        case Colors::bg_two:
            return juce::Colour (66, 67, 74);
        case Colors::accent:
            return juce::Colour (31, 115, 255);
        case Colors::accent_two:
            return juce::Colour (44, 40, 93);
        case Colors::clip:
            return juce::Colour (255, 23, 68);
        case Colors::font:
            return juce::Colour (255, 255, 255);
        case Colors::lcd:
            return juce::Colour (0, 129, 194);
        case Colors::lcd_bg:
            return juce::Colour (0, 0, 0);
        case Colors::unknown:
        default:
            return juce::Colour (255, 255, 255);
    }
}

} // namespace VeNo::Theme