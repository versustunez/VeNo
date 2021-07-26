#include <VeNo/Core/Config.h>
#include <VeNo/Definitions.h>
#include <VeNo/GUI/GUIUtils.h>
#include <VeNo/PluginEditor.h>
#include <VeNo/PluginProcessor.h>
#include <string>
#include <utility>

VeNoEditor::VeNoEditor (VeNoProcessor& p, std::string id)
    : AudioProcessorEditor (&p), m_id (std::move (id)), m_instanceId (p.instance->id)
{
    auto& config = VeNo::Core::Config::get();
    config.registerEditor (m_id, this);
    mainInterpreter = &VeNo::Core::Instance::get(m_instanceId)->mainInterpreter;
    mainInterpreter->parseMain (config.guiInit.get ("Main"));
    auto& pos = mainInterpreter->componentGroup->position();
    int width = pos.w > 0 ? pos.w : WINDOW_WIDTH;
    int height = pos.h > 0 ? pos.h : WINDOW_HEIGHT;
    setResizable (false, false);
    setSize (VeNo::GUI::Utils::getScaledSize (width), VeNo::GUI::Utils::getScaledSize (height));
    addAndMakeVisible (mainInterpreter->componentGroup.get());
    if (config.getProperties()->getBoolValue ("useOpenGL", true))
        setupGL (config.getProperties()->getBoolValue ("vsync", true));
}

void VeNoEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0, 0, 0));
}
VeNoEditor::~VeNoEditor()
{
    VeNo::Core::Config::get().removeEditor (m_id);
    m_openGLContext.detach();
}
void VeNoEditor::resized()
{
    mainInterpreter->componentGroup->setBounds (0, 0, getWidth(), getHeight());
}
void VeNoEditor::setupGL (bool vsync)
{
    DBGN ("Use OpenGL Rendering")
    m_openGLContext.setRenderer (this);
    if (! vsync && ! m_openGLContext.setSwapInterval (0))
        WARN ("Cannot deactivate VSync")
    m_openGLContext.setContinuousRepainting (true);

    m_openGLContext.attachTo (*this);
    m_openGLContext.makeActive();
}
void VeNoEditor::newOpenGLContextCreated()
{
}
void VeNoEditor::renderOpenGL()
{
    auto knobs = VeNo::Core::Instance::get (m_instanceId)->state.components.m_knobs;
    for (auto& knob : knobs)
        if (knob.second->isVisible())
            knob.second->repaint();
}
void VeNoEditor::openGLContextClosing()
{
}
