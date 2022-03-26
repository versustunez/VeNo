#include <VeNo/GUI/Theme/Theme.h>
#include <utility>

namespace VeNo::Theme {
Theme::Theme(Ref<Core::Properties> file) : m_configFile(std::move(file)) {}
Theme::~Theme() {
  m_configFile.reset();
  for (auto &item : m_colours)
    delete item.second;
}

void Theme::init() {
  getColourFromConfig(Colors::bg);
  getColourFromConfig(Colors::bgTwo);
  getColourFromConfig(Colors::accent);
  getColourFromConfig(Colors::accentTwo);
  getColourFromConfig(Colors::font);
  getColourFromConfig(Colors::clip);
  getColourFromConfig(Colors::LcdBg);
  getColourFromConfig(Colors::lcd);
  getColourFromConfig(Colors::logo);
  getColourFromConfig(Colors::logoAccent);
  getColourFromConfig(Colors::root);

  m_colorMapping["primaryBG"] = Colors::bg;
  m_colorMapping["secondaryBG"] = Colors::bgTwo;
  m_colorMapping["primary"] = Colors::accent;
  m_colorMapping["secondary"] = Colors::accentTwo;
  m_colorMapping["font"] = Colors::font;
  m_colorMapping["clip"] = Colors::clip;
  m_colorMapping["lcdBG"] = Colors::LcdBg;
  m_colorMapping["lcd"] = Colors::lcd;
  m_colorMapping["logo"] = Colors::logo;
  m_colorMapping["logoAccent"] = Colors::logoAccent;
  m_colorMapping["rootBG"] = Colors::root;
}
juce::Colour Theme::getColor(Colors index) {
  if (m_colours[index] != nullptr)
    return *m_colours[index];
  return {255, 255, 255};
}
void Theme::setColor(Colors index, juce::Colour *colour) {
  delete m_colours[index];
  m_colours[index] = colour;
}
void Theme::getColourFromConfig(Colors index) {
  std::string key = colorToString(index);
  delete m_colours[index];
  auto color = getDefault(index);
  m_colours[index] = new juce::Colour(juce::Colour::fromString(
      m_configFile->asString(key, color.toString().toStdString())));
}
Colors Theme::getColorIndex(const std::string &color) {
  if (m_colorMapping.find(color) != m_colorMapping.end()) {
    return m_colorMapping[color];
  }
  return Colors::unknown;
}
std::string Theme::colorToString(Colors index) {
  switch (index) {
  case Colors::bg: return "color.primaryBG";
  case Colors::bgTwo: return "color.secondaryBG";
  case Colors::accent: return "color.primary";
  case Colors::accentTwo: return "color.secondary";
  case Colors::clip: return "color.clip";
  case Colors::font: return "color.font";
  case Colors::LcdBg: return "color.lcdBG";
  case Colors::lcd: return "color.lcd";
  case Colors::logo: return "color.logo";
  case Colors::logoAccent: return "color.logoPrimary";
  case Colors::root: return "color.rootBG";
  case Colors::unknown:
  default: return "";
  }
}
juce::Colour Theme::getDefault(Colors index) {
  switch (index) {
  case Colors::bg: return {27, 27, 33};
  case Colors::bgTwo: return {66, 67, 74};
  case Colors::accent: return {31, 115, 255};
  case Colors::accentTwo: return {44, 40, 93};
  case Colors::clip: return {255, 23, 68};
  case Colors::font: return {255, 255, 255};
  case Colors::lcd: return {0, 129, 194};
  case Colors::LcdBg: return {0, 0, 0};
  case Colors::logo: return {255, 255, 255};
  case Colors::logoAccent: return {31, 115, 255};
  case Colors::root: return {11, 11, 11};
  case Colors::unknown:
  default: return {255, 255, 255};
  }
}
} // namespace VeNo::Theme