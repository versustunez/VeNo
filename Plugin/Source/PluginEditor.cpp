#include "PluginProcessor.h"
#include "PluginEditor.h"

VeNoEditor::VeNoEditor(VeNoProcessor& p)
    : AudioProcessorEditor(&p)
{
    setSize(400, 300);
}

void VeNoEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}
