#pragma once
#include <JuceHeader.h>
#include <VeNo/GUI/Components/Components.h>

namespace VeNo::GUI {

class VeNoLook : public juce::LookAndFeel_V4 {
public:
  virtual void drawValueBox(juce::Graphics &g, int x, int y, int width,
                            int height, juce::Slider &slider, Knob *knob) = 0;
};
} // namespace VeNo::GUI