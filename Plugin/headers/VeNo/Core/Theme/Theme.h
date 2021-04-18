#pragma once

#include <JuceHeader.h>
#include <string>
#include <vendor/tsl/robin_map.h>

namespace VeNo::Theme
{
enum class Colors
{
    unknown = 0,
    bg,
    bg_two,
    accent,
    accent_two,
    font,
    clip,
    lcd_bg,
    lcd
};

class Theme
{
public:
    explicit Theme(std::shared_ptr<juce::PropertiesFile> file);
    void init();
    juce::Colour getDirectColor(Colors index);
    juce::Colour getColor(const std::string& name);
    void setColor(Colors index, juce::Colour* colour);
    static juce::Colour getDefault (Colors index);
protected:
    void getColourFromConfig (Colors index);
    static std::string colorToString (Colors index);
    tsl::robin_map<Colors, juce::Colour*> m_colours;
    std::shared_ptr<juce::PropertiesFile> m_configFile;
    tsl::robin_map<std::string, Colors> m_colorMapping = {};
};
} // namespace VeNo::Theme