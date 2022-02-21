#pragma once
#include <JuceHeader.h>
#include <VeNo/TypeDefs.h>

namespace VeNo::GUI {
class Fonts {
protected:
  static Ref<Fonts> instance;
  juce::Font *lcdFont;
  juce::Font *arvo;
  juce::Font *icons;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Fonts)
public:
  Fonts();

  ~Fonts();

  static void destroyAll();

  static juce::Font *getLCD();

  static juce::Font *getNormal();

  static Fonts *getInstance();

  static juce::Font *getIcons();

  static void drawIcon(const juce::String &icon, juce::Graphics &g,
                       juce::Rectangle<int> &space);
};
} // namespace VeNo::GUI
