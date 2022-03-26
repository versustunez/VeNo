#include "VeNo/GUI/Components/Button.h"
#include "VeNo/GUI/Fonts/Fonts.h"

#include <VeNo/Core/Config.h>
#include <VeNo/GUI/GUIUtils.h>
#include <VeNo/GUI/Theme/LookAndFeel/LookHandler.h>
#include <cassert>

namespace VeNo::GUI {
typedef VeNo::Theme::Colors Colors;
LookHandler::LookHandler() {
  theme = Core::Config::get().theme().get();
  setDefaultLookAndFeel(this);
}

LookHandler::~LookHandler() {
  DBGN("Delete LookHandler");
  delete m_feels[0];
  m_feels[0] = nullptr;
  setDefaultLookAndFeel(nullptr);
}

void LookHandler::selectLook(int index) { m_currentLook = index; }

VeNoLook *LookHandler::getLook() {
  assert(m_feels[0] != nullptr);
  return m_feels[m_currentLook];
}

void LookHandler::drawButtonBackground(juce::Graphics &g, juce::Button &button,
                                       const juce::Colour &backgroundColour,
                                       bool shouldDrawButtonAsHighlighted,
                                       bool shouldDrawButtonAsDown) {
  getLook()->drawButtonBackground(g, button, backgroundColour,
                                  shouldDrawButtonAsHighlighted,
                                  shouldDrawButtonAsDown);
}

void LookHandler::drawRotarySlider(juce::Graphics &g, int x, int y, int width,
                                   int height, float sliderPosProportional,
                                   float rotaryStartAngle, float rotaryEndAngle,
                                   juce::Slider &slider) {

  drawBasedOnKnob(g, x, y, width, height, sliderPosProportional,
                  rotaryStartAngle, rotaryEndAngle, slider);
}

void LookHandler::drawTextEditorOutline(juce::Graphics &graphics, int width,
                                        int height, juce::TextEditor &editor) {
  getLook()->drawTextEditorOutline(graphics, width, height, editor);
}

void LookHandler::drawToggleButton(juce::Graphics &g,
                                   juce::ToggleButton &button,
                                   bool shouldDrawButtonAsHighlighted,
                                   bool shouldDrawButtonAsDown) {
  getLook()->drawToggleButton(g, button, shouldDrawButtonAsHighlighted,
                              shouldDrawButtonAsDown);
}

void LookHandler::drawTabButton(juce::TabBarButton &btn, juce::Graphics &g,
                                bool isMouseOver, bool isMouseDown) {
  getLook()->drawTabButton(btn, g, isMouseOver, isMouseDown);
}

void LookHandler::drawComboBox(juce::Graphics &g, int width, int height,
                               bool isButtonDown, int buttonX, int buttonY,
                               int buttonW, int buttonH, juce::ComboBox &box) {
  getLook()->drawComboBox(g, width, height, isButtonDown, buttonX, buttonY,
                          buttonW, buttonH, box);
}

void LookHandler::drawLabel(juce::Graphics &graphics, juce::Label &label) {
  getLook()->drawLabel(graphics, label);
}

void LookHandler::drawTabAreaBehindFrontButton(juce::TabbedButtonBar &,
                                               juce::Graphics &g, int w,
                                               int h) {
  g.setColour(theme->getColor(Colors::bgTwo).withAlpha(0.5f));
  g.drawLine(0, h, w, h);
}

juce::PopupMenu::Options
LookHandler::getOptionsForComboBoxPopupMenu(juce::ComboBox &box,
                                            juce::Label &) {
  return juce::PopupMenu::Options()
      .withTargetComponent(&box)
      .withItemThatMustBeVisible(box.getSelectedId())
      .withMinimumWidth(box.getWidth())
      .withMaximumNumColumns(3)
      .withStandardItemHeight(25);
}

void LookHandler::drawPopupMenuItem(
    juce::Graphics &g, const juce::Rectangle<int> &area, bool isSeparator,
    bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu,
    const juce::String &text, const juce::String &, const juce::Drawable *,
    const juce::Colour *) {
  if (isSeparator) {
    auto r = area.reduced(5, 0);
    r.removeFromTop(juce::roundToInt(((float)r.getHeight() * 0.5f) - 0.5f));

    g.setColour(findColour(juce::PopupMenu::textColourId).withAlpha(0.3f));
    g.fillRect(r.removeFromTop(1));
  } else {
    auto textColour = theme->getColor(Colors::font);
    auto r = area.reduced(1);
    if (isHighlighted && isActive) {
      g.setColour(theme->getColor(Colors::accent).withAlpha(0.4f));
      g.fillRect(r);
      g.setColour(theme->getColor(Colors::font));
    } else {
      g.setColour(textColour.withMultipliedAlpha(isActive ? 1.0f : 0.5f));
    }
    auto font = getPopupMenuFont();
    auto maxFontHeight = (float)r.getHeight() / 1.3f;
    if (font.getHeight() > maxFontHeight)
      font.setHeight(maxFontHeight);

    g.setFont(font);
    if (hasSubMenu) {
      auto arrowH = 0.6f * getPopupMenuFont().getAscent();

      auto x = static_cast<float>(r.removeFromRight((int)arrowH).getX());
      auto halfH = static_cast<float>(r.getCentreY());

      juce::Path path;
      path.startNewSubPath(x, halfH - arrowH * 0.5f);
      path.lineTo(x + arrowH * 0.6f, halfH);
      path.lineTo(x, halfH + arrowH * 0.5f);

      g.strokePath(path, juce::PathStrokeType(2.0f));
    }

    r.removeFromRight(3);
    r.removeFromLeft(10);
    g.drawFittedText(text, r,
                     hasSubMenu ? juce::Justification::centred
                                : juce::Justification::centredLeft,
                     1);

    if (isTicked) {
      g.setColour(theme->getColor(Colors::accent));
      g.fillRect(0, 0, 5, r.getHeight());
    }
  }
}

void LookHandler::drawPopupMenuBackground(juce::Graphics &g, int width,
                                          int height) {
  g.fillAll(theme->getColor(Colors::bg));
  juce::ignoreUnused(width, height);

#if !JUCE_MAC
  g.setColour(theme->getColor(Colors::bg).withAlpha(0.2f));
  g.drawRect(0, 0, width, height);
#endif
}

void LookHandler::drawBasedOnKnob(juce::Graphics &g, int x, int y, int width,
                                  int height, float sliderPosProportional,
                                  float rotaryStartAngle, float rotaryEndAngle,
                                  juce::Slider &slider) {
  auto *knob = Utils::instanceOf<Knob>(slider.getParentComponent());
  if (knob != nullptr && knob->isValueBox()) {
    getLook()->drawValueBox(g, x, y, width, height, slider, knob);
  } else {
    getLook()->drawRotarySlider(g, x, y, width, height, sliderPosProportional,
                                rotaryStartAngle, rotaryEndAngle, slider);
  }
}
void LookHandler::drawTooltip(juce::Graphics &graphics,
                              const juce::String &text, int width, int height) {

  LookAndFeel_V4::drawTooltip(graphics, text, width, height);
}
void LookHandler::drawBubble(juce::Graphics &graphics,
                             juce::BubbleComponent &component,
                             const juce::Point<float> &tip,
                             const juce::Rectangle<float> &body) {
  getLook()->drawBubble(graphics, component, tip, body);
}
void LookHandler::drawButtonText(juce::Graphics &graphics,
                                 juce::TextButton &button, bool, bool) {
  auto *buttonOrNull = Utils::instanceOf<Button>(button.getParentComponent());
  if (buttonOrNull && buttonOrNull->isIconButton()) {
    auto font = graphics.getCurrentFont();
    graphics.setColour(theme->getColor(Colors::font));
    graphics.setFont(*Fonts::getIcons());
    auto rect =
        juce::Rectangle<int>(0, 0, button.getWidth(), button.getHeight());
    graphics.drawText(button.getButtonText(), rect,
                      juce::Justification::centred, true);
    graphics.setFont(font);
  } else {
    graphics.setFont(Fonts::getNormal()->getTypefacePtr());
    graphics.setColour(theme->getColor(Colors::font));
    juce::Rectangle<int> rect{0, 0, button.getWidth(), button.getHeight()};
    graphics.drawText(button.getButtonText(), rect,
                      juce::Justification::centred, true);
  }
}
void LookHandler::drawTabbedButtonBarBackground(juce::TabbedButtonBar &bar,
                                                juce::Graphics &graphics) {
  graphics.setColour(theme->getColor(Colors::bg));
  graphics.fillAll();
}
} // namespace VeNo::GUI