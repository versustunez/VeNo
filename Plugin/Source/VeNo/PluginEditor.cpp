#include <VeNo/Core/Config.h>
#include <VeNo/Definitions.h>
#include <VeNo/PluginEditor.h>
#include <VeNo/PluginProcessor.h>
#include <string>
#include <utility>

VeNoEditor::VeNoEditor (VeNoProcessor& p, std::string id)
    : AudioProcessorEditor (&p), m_id (std::move (id)), mainInterpreter (VeNo::GUI::Interpreter(p.instance->id))
{
    auto& config = VeNo::Core::Config::get();
    config.registerEditor (m_id, this);
    mainInterpreter.parseMain(config.guiInit.get("Main"));
    setSize (WINDOW_WIDTH, WINDOW_HEIGHT);
    addAndMakeVisible(mainInterpreter.componentGroup.get());
}

void VeNoEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}
VeNoEditor::~VeNoEditor()
{
    VeNo::Core::Config::get().removeEditor (m_id);
}
void VeNoEditor::resized()
{
    mainInterpreter.componentGroup->setBounds(0,0,getWidth(),getHeight());
}
