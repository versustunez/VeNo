#pragma once

#include "VeNo/Core/PropertiesWrapper.h"

#include <JuceHeader.h>
#include <VeNo/TypeDefs.h>
#include <string>

namespace VeNo::Theme {
enum class Colors {
  unknown = 0,
  root,
  bg,
  bgTwo,
  accent,
  accentTwo,
  font,
  clip,
  lcdBg,
  lcd,
  logo,
  logoAccent,
  end
};

class Theme {
public:
  explicit Theme(Ref<Core::Properties> file);
  ~Theme();
  void init();
  juce::Colour getColor(Colors index);
  void setColor(Colors index, juce::Colour *colour);
  static juce::Colour getDefault(Colors index);

  Colors getColorIndex(const std::string &color);
  static std::string colorName(Colors index);
  static juce::Colour getContrastColor(const juce::Colour &colour);

protected:
  void getColourFromConfig(Colors index);
  static std::string colorToString(Colors index);
  Map<Colors, juce::Colour *> m_colours;
  Ref<Core::Properties> m_configFile;
  Map<std::string, Colors> m_colorMapping = {};
};
} // namespace VeNo::Theme