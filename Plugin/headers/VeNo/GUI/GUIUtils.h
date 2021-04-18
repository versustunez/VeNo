#pragma once

#include <JuceHeader.h>

namespace VeNo::GUI {
class Utils {
public:
    static int getScaledSize (int size);
    static double setFontSize (double size, juce::Graphics& g);
    static juce::Path drawRectangleWithRadius (int x, int y, int w, int h, float radius, juce::Graphics& g);
};
}