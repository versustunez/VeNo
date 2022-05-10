#include "LCD.h"

#include "VeNo/Core/Config.h"

namespace VeNo::GUI {
LCDComponent::LCDComponent(const std::string &name, const std::string &showName,
                           unsigned long id)
    : BaseComponent(name, showName, id) {

  m_header = Make<LCDHeader>(name, showName);
  m_info = Make<LCDInfo>(name, showName);
  m_toolbar = Make<LCDToolbar>(name, showName);
  m_theme = Core::Config::get().theme().get();
}
void LCDComponent::resized() {
  int yOffset = 0;
  int headerToolbarHeight = (int)std::floor(getHeight() * 0.12);
  int infoHeight = getHeight() - (headerToolbarHeight * 2);
  m_header->setBounds(0, yOffset, getWidth(), headerToolbarHeight);
  yOffset += headerToolbarHeight;
  m_info->setBounds(0, yOffset, getWidth(), infoHeight);
  yOffset += infoHeight;
  m_toolbar->setBounds(0, yOffset, getWidth(), headerToolbarHeight);
}
void LCDComponent::paint(juce::Graphics &g) {
  g.fillAll(m_theme->getColor(Theme::Colors::lcdBg));
}
} // namespace VeNo::GUI
