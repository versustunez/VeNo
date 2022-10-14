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
    g.strokePath(m_DrawPath, juce::PathStrokeType(1.0f));
    break;
  case LCDInfoState::STEREO: g.fillPath(m_DrawPath); break;
  case LCDInfoState::STEREO_TWO: drawStereoTwo(g); break;
  case LCDInfoState::LOGO_ONLY: drawLogo(g); break;
  }
}

void LCDInfo::drawVU(juce::Graphics &g) {

  auto getScale = [&](double dB) {
    double fDef = (dB + 30) / 30;
    return int(fDef * 0.95 * getHeight());
  };
  g.fillPath(m_DrawPath);
  g.setFont(9.0);
  g.setFont(Fonts::getLCD()->getTypefacePtr());
  drawVULabels(g, getScale(0), "-0-");
  drawVULabels(g, getScale(-3), "-3-");
  drawVULabels(g, getScale(-6), "-6-");
  drawVULabels(g, getScale(-9), "-9-");
  drawVULabels(g, getScale(-12), "-12-");
  drawVULabels(g, getScale(-18), "-18-");
  drawVULabels(g, getScale(-27), "-27-");
}

void LCDInfo::drawVULabels(juce::Graphics &g, int y, const std::string &label) {
  auto font = g.getCurrentFont();
  int currentY = getHeight() - y;
  int thisWidth = getWidth();

  int iMidHeight = (int)(font.getHeight() * 0.5f);

  g.drawText(label, 2, currentY - iMidHeight, thisWidth - 3,
             (int)font.getHeight(), juce::Justification::centred, false);
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
void LCDInfo::mouseDown(const juce::MouseEvent &) {
  switch (m_state) {
  case LCDInfoState::VU: m_state = LCDInfoState::WAVE; break;
  case LCDInfoState::WAVE: m_state = LCDInfoState::STEREO; break;
  case LCDInfoState::STEREO: m_state = LCDInfoState::STEREO_TWO; break;
  case LCDInfoState::STEREO_TWO: m_state = LCDInfoState::LOGO_ONLY; break;
  case LCDInfoState::LOGO_ONLY: m_state = LCDInfoState::VU; break;
  }
  updateData();
  triggerAsyncUpdate();
}

std::tuple<double, double> getStereoData(double x, double y) {
  double radius = sqrt((x * x) + (y * y));
  double angle = atan(y / x);
  if ((x < 0 && y > 0) || (x < 0 && y < 0))
    angle += 3.14159265;
  else if (x > 0 && y < 0)
    angle += 6.28318530;
  angle = x != 0 ? angle : y > 0 ? 1.57079633 : 4.71238898;
  angle = y != 0 ? angle : x > 0 ? 0 : 3.14159265;
  angle += 0.78539816;
  return {radius * cos(angle), radius * sin(angle)};
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
    double radius;
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;
    for (size_t i = 0; i < leftChannel.size(); ++i) {
      double x = std::clamp(leftChannel[i], -1.0, 1.0);
      double y = std::clamp(rightChannel[i], -1.0, 1.0);
      radius = std::sqrt(x * x + y * y);
      double theta = std::atan(y / x);
      double angle = theta * (180.0/juce::MathConstants<double>::pi);
      double radX = radius * std::cos(angle);
      double radY = radius * std::sin(angle);
      m_DrawPath.addRectangle(radX * halfWidth + halfWidth, radY * halfHeight + halfHeight, 0.5, 0.5);
    }
  } else if (m_state == LCDInfoState::STEREO_TWO) {
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;
    for (size_t i = 0; i < leftChannel.size(); ++i) {
      auto [x, y] = getStereoData(leftChannel[i], rightChannel[i]);
      m_DrawPath.addRectangle((x * halfWidth) + halfWidth,
                              (y * halfHeight) + halfHeight, 0.5, 0.5);
    }
  }
}

void LCDInfo::drawStereoTwo(juce::Graphics &graphics) {
  auto color = m_theme->getColor(Theme::Colors::lcd);
  graphics.setColour(color.withAlpha(0.2f));
  auto width = (float)getWidth();
  auto height = (float)getHeight();
  float halfWidth = width / 2.0f;
  float halfHeight = height / 2.0f;
  graphics.drawLine(10, halfHeight, width - 10, halfHeight);
  graphics.drawLine(halfWidth, 10, halfWidth, height - 10);
  graphics.setColour(color);
  graphics.fillPath(m_DrawPath);
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