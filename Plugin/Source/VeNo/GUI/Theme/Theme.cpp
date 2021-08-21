#include <VeNo/GUI/Theme/Theme.h>

#include <utility>

namespace VeNo::Theme
{
Theme::Theme (std::shared_ptr<juce::PropertiesFile> file)
    : m_configFile (std::move (file))
{
}
Theme::~Theme()
{
    m_configFile.reset();
    for (auto& item : m_colours)
        delete item.second;
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
    return {255, 255, 255};
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
        m_colours[index] = new juce::Colour (juce::Colour::fromString (m_configFile->getValue (key)));
    else {
        auto color = getDefault (index);
        m_configFile->setValue(key, color.toString());
        m_colours[index] = new juce::Colour (color);
    }
}
bool Theme::colorExists(const std::string& color) {
    return m_colorMapping.find(color) != m_colorMapping.end();
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
            return {27, 27, 33};
        case Colors::bg_two:
            return {66, 67, 74};
        case Colors::accent:
            return {31, 115, 255};
        case Colors::accent_two:
            return {44, 40, 93};
        case Colors::clip:
            return {255, 23, 68};
        case Colors::font:
            return {255, 255, 255};
        case Colors::lcd:
            return {0, 129, 194};
        case Colors::lcd_bg:
            return {0, 0, 0};
        case Colors::unknown:
        default:
            return {255, 255, 255};
    }
}
} // namespace VeNo::Theme