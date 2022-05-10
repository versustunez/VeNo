#include "LCDSubComponents.h"

#include "VeNo/Core/Config.h"
#include "VeNo/GUI/Fonts/Fonts.h"

namespace VeNo::GUI {

// LCD Header
LCDHeader::LCDHeader(const std::string &name, const std::string &showName,
                     InstanceID id)
    : BaseComponent(name, showName, id) {}

void LCDHeader::paint(juce::Graphics &g) { g.fillAll(juce::Colour(255, 0, 0)); }

// LCD INFO SCREEN

LCDInfo::LCDInfo(const std::string &name, const std::string &showName,
                 InstanceID id)
    : BaseComponent(name, showName, id) {
  m_theme = Core::Config::get().theme().get();
  m_handler->addHandler("lcd-change", this);
}

LCDInfo::~LCDInfo() { m_handler->removeHandler("lcd-change"); }

void LCDInfo::paint(juce::Graphics &g) {
  juce::Rectangle<int> space{0, 0, getWidth(), getHeight()};
  g.fillAll(m_theme->getColor(Theme::Colors::lcdBg));
  g.setColour(m_theme->getColor(Theme::Colors::lcd));
  switch (m_state) {
  case LCDInfoState::VU: drawVU(g); break;
  case LCDInfoState::WAVE: drawWave(g); break;
  case LCDInfoState::STEREO: drawStereo(g); break;
  case LCDInfoState::ONCHANGE: drawChange(g); break;
  case LCDInfoState::NOTHING: break;
  }
}

void LCDInfo::drawVU(juce::Graphics &g) {}
void LCDInfo::drawStereo(juce::Graphics &g) {}
void LCDInfo::drawWave(juce::Graphics &g) {}

void LCDInfo::drawChange(juce::Graphics &g) {
  g.setFont(Fonts::getLCD()->getTypefacePtr());
  auto fontSize = (float)getHeight() / 5.0f;
  auto mid = ((float)getHeight() / 2.0f) - fontSize;
  g.setFont(fontSize);
  if (!m_changedParameter.name.empty()) {
    // Parameter name
    g.drawText(m_changedParameter.name, 0, (int)mid, getWidth(), (int)fontSize,
               juce::Justification::centred, true);
    // Parameter value!
    auto valueText =
        VUtils::StringUtils::toString(m_changedParameter.value, 3, true);
    g.drawText(valueText, 0, (int)mid + (int)fontSize, getWidth(),
               (int)fontSize, juce::Justification::centred, true);
  } else {
    g.drawText("[-- VeNo --]", 0, (int)mid, getWidth(), (int)fontSize,
               juce::Justification::centred, true);
    g.setFont(fontSize / 2.0f);
    g.drawText("The Supportive Synth", 0, (int)mid + (int)fontSize, getWidth(),
               (int)fontSize, juce::Justification::centred, true);
  }
  g.setFont(Fonts::getNormal()->getTypefacePtr());
}

void LCDInfo::handle(Events::Event *event) {
  if (m_state == LCDInfoState::NOTHING) {
    return;
  }
  if (m_state == LCDInfoState::ONCHANGE) {
    if (event->is<Events::ParameterChange>()) {
      auto *parameterChange = event->as<Events::ParameterChange>();
      m_changedParameter.name = parameterChange->name;
      m_changedParameter.value = parameterChange->value;
    } else if (event->is<Events::ParameterChangeEnd>()) {
      m_changedParameter.name = "";
    } else if (event->is<Events::ValueChange>()) {
      auto *parameterChange = event->as<Events::ValueChange>();
      m_changedParameter.value = parameterChange->value;
    }
  } else {
  }
  triggerAsyncUpdate();
}
void LCDInfo::handleAsyncUpdate() { repaint(); }

// LCD TOOLBAR
LCDToolbar::LCDToolbar(const std::string &name, const std::string &showName,
                       InstanceID id)
    : BaseComponent(name, showName, id) {}

void LCDToolbar::paint(juce::Graphics &g) {
  g.fillAll(juce::Colour(0, 0, 255));
}

} // namespace VeNo::GUI