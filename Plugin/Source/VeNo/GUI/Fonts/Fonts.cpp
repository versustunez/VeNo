#include <Assets.h>
#include <VUtils/Logging.h>
#include <VeNo/GUI/Fonts/Fonts.h>
#include <VeNo/TypeDefs.h>

namespace VeNo::GUI {
Ref<Fonts> Fonts::instance = nullptr;

juce::Font *Fonts::getNormal() { return getInstance()->arvo; }

juce::Font *Fonts::getLCD() { return getInstance()->lcdFont; }

juce::Font *Fonts::getIcons() { return getInstance()->icons; }

Fonts *Fonts::getInstance() {
  if (instance == nullptr)
    instance = CreateRef<Fonts>();
  return instance.get();
}

void Fonts::destroyAll() { instance = nullptr; }

Fonts::Fonts() {
  arvo = new juce::Font(juce::Typeface::createSystemTypefaceFor(
      BinaryData::arvo_ttf, BinaryData::arvo_ttfSize));
  lcdFont = new juce::Font(juce::Typeface::createSystemTypefaceFor(
      BinaryData::lcd_TTF, BinaryData::lcd_TTFSize));
  icons = new juce::Font(juce::Typeface::createSystemTypefaceFor(
      BinaryData::fontAwesome_ttf, BinaryData::fontAwesome_ttfSize));
}

Fonts::~Fonts() {
  delete arvo;
  delete lcdFont;
  delete icons;
  arvo = nullptr;
  lcdFont = nullptr;
  icons = nullptr;
}

void Fonts::drawIcon(const juce::String &icon, juce::Graphics &g,
                     juce::Rectangle<int> &space) {
  auto font = g.getCurrentFont();
  g.setFont(*Fonts::getIcons());
  g.drawText(icon, space, juce::Justification::centred, true);
  g.setFont(font);
}
} // namespace VeNo::GUI
