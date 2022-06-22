#include "LCDSubComponents.h"

#include "VUtils/Math.h"
#include "VeNo/Core/Config.h"
#include "VeNo/Core/Instance.h"
#include "VeNo/GUI/Fonts/Fonts.h"

namespace VeNo::GUI {

// LCD Header
LCDHeader::LCDHeader(const std::string &name, const std::string &showName,
                     InstanceID id)
    : BaseComponent(name, showName, id) {}

void LCDHeader::paint(juce::Graphics &g) {
  g.setFont(Fonts::getLCD()->getTypefacePtr());
  g.setColour(Core::Config::get().theme()->getColor(Theme::Colors::lcd));
  int line = getHeight() - 2;
  g.drawText("LICENSED TO: " + juce::SystemStats::getFullUserName(), 0, 2,
             getWidth(), getHeight() - 4, juce::Justification::centred, true);
  g.drawLine(0, line, getWidth(), line);
}

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
  if (!m_changedParameter.name.empty()) {
    drawChange(g);
    return;
  }
  switch (m_state) {
  case LCDInfoState::VU: drawVU(g); break;
  case LCDInfoState::WAVE:
  case LCDInfoState::STEREO:
    g.strokePath(m_DrawPath, juce::PathStrokeType(1.0f));
    break;
  case LCDInfoState::LOGO_ONLY: drawLogo(g); break;
  }
}

void LCDInfo::drawVU(juce::Graphics &g) {
  g.fillPath(m_DrawPath);
}

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
  }
  g.setFont(Fonts::getNormal()->getTypefacePtr());
}

void LCDInfo::drawLogo(juce::Graphics &g) {
  g.setFont(Fonts::getLCD()->getTypefacePtr());
  auto fontSize = (float)getHeight() / 5.0f;
  auto mid = ((float)getHeight() / 2.0f) - fontSize;
  g.setFont(fontSize);
  g.drawText("[-- VeNo --]", 0, (int)mid, getWidth(), (int)fontSize,
             juce::Justification::centred, true);
  g.setFont(fontSize / 2.0f);
  g.drawText("The Supportive Synth", 0, (int)mid + (int)fontSize, getWidth(),
             (int)fontSize, juce::Justification::centred, true);
  g.setFont(Fonts::getNormal()->getTypefacePtr());
}

void LCDInfo::handle(Events::Event *event) {
  if (event->is<Events::ParameterChange>()) {
    auto *parameterChange = event->as<Events::ParameterChange>();
    m_changedParameter.name = parameterChange->name;
    m_changedParameter.value = parameterChange->value;
  } else if (event->is<Events::ParameterChangeEnd>()) {
    m_changedParameter.name = "";
  } else if (event->is<Events::ValueChange>()) {
    auto *parameterChange = event->as<Events::ValueChange>();
    m_changedParameter.value = parameterChange->value;
  } else if (event->is<Events::TriggerEvent>()) {
    // nice :D
    updateData();
  }
  triggerAsyncUpdate();
}
void LCDInfo::handleAsyncUpdate() { repaint(); }
void LCDInfo::mouseDown(const juce::MouseEvent &event) {
  switch (m_state) {
  case LCDInfoState::VU: m_state = LCDInfoState::WAVE; break;
  case LCDInfoState::WAVE: m_state = LCDInfoState::STEREO; break;
  case LCDInfoState::STEREO: m_state = LCDInfoState::LOGO_ONLY; break;
  case LCDInfoState::LOGO_ONLY: m_state = LCDInfoState::VU; break;
  }
  updateData();
  triggerAsyncUpdate();
}

void LCDInfo::updateData() {
  auto &buffer = Core::Instance::get(m_id)->buffer;
  auto leftChannel = buffer->getChannel(0);
  auto rightChannel = buffer->getChannel(1);
  m_DrawPath.clear();
  auto height = (float)getHeight();
  auto width = (float)getWidth();
  if (m_state == LCDInfoState::VU) {
    auto findPeak = [&](const Vector<double> &innerBuffer) {
      double sum = 0.0;
      for (double i : innerBuffer) {
        sum += i * i;
      }
      double val = std::sqrt(sum / (double)innerBuffer.size());
      double max = VUtils::Math::clamp(
          juce::Decibels::gainToDecibels(val, -30.0), -30.0, 0);
      return VUtils::Math::map(max, -30.0f, 0.0f, 1.0, 0.1f);
    };
    auto leftPeak = findPeak(leftChannel);
    auto rightPeak = findPeak(rightChannel);
    float xLeft1 = 0.05f * width;
    float xLeft2 = 0.46f * width;
    m_DrawPath.addTriangle(xLeft1, (float)leftPeak * height, xLeft1, height,
                           xLeft2, height);
    // leftChannel!
    xLeft1 = 0.95f * width; // from pos -1,1 0.9
    xLeft2 = 0.54f * width; // from pos -1,1 0.08
    m_DrawPath.addTriangle(xLeft1, (float)rightPeak * height, xLeft1, height,
                           xLeft2, height);
  } else if (m_state == LCDInfoState::WAVE) {
    float x = 0;
    float xInc = width / (float)leftChannel.size();
    m_DrawPath.startNewSubPath(0, height * 0.5f);
    for (size_t i = 0; i < leftChannel.size(); ++i) {
      auto value = (leftChannel[i] + rightChannel[i]) * 0.5;
      m_DrawPath.lineTo(x + xInc, (float)(value + 1.0f) * 0.5f * height);
      x += xInc;
    }
  } else if (m_state == LCDInfoState::STEREO) {
    auto leftData = (leftChannel[0] + 1.0) * 0.5;
    auto rightData = (rightChannel[0] + 1.0) * 0.5;
    m_DrawPath.startNewSubPath((float)leftData * width,
                               height - ((float)rightData * height));
    for (size_t i = 0; i < leftChannel.size(); ++i) {
      leftData = (leftChannel[i] + 1.0) * 0.5;
      rightData = (rightChannel[i] + 1.0) * 0.5;
      m_DrawPath.lineTo((float)leftData * width,
                        height - ((float)rightData * height));
    }
    m_DrawPath.closeSubPath();
  }
}

// LCD TOOLBAR
LCDToolbar::LCDToolbar(const std::string &name, const std::string &showName,
                       InstanceID id)
    : BaseComponent(name, showName, id) {}

void LCDToolbar::paint(juce::Graphics &g) {
  g.setFont(Fonts::getLCD()->getTypefacePtr());
  g.setColour(Core::Config::get().theme()->getColor(Theme::Colors::lcd));
  g.drawText(VeNo_SCREEN_TEXT, 0, 2, getWidth(), getHeight() - 4,
             juce::Justification::centred, true);
  g.drawLine(0, 0, getWidth(), 2);
}

} // namespace VeNo::GUI