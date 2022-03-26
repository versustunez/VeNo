#include <VeNo/Core/Config.h>
#include <VeNo/Definitions.h>
#include <VeNo/GUI/GUIUtils.h>
#include <VeNo/PluginEditor.h>
#include <VeNo/Utils/ProfileMacros.h>

VeNoEditor::VeNoEditor(VeNoProcessor &p, std::string id)
    : AudioProcessorEditor(&p),
      m_id(std::move(id)),
      m_instanceId(p.instance->id) {
  auto &config = VeNo::Core::Config::get();
  config.registerEditor(m_id, this);
  auto *instance = VeNo::Core::Instance::get(m_instanceId);
  mainInterpreter = instance->mainInterpreter;
  mainInterpreter->parseMain(config.guiInit.get("Main"));
  auto &pos = mainInterpreter->componentGroup->position();
  int width = pos.w > 0 ? pos.w : WINDOW_WIDTH;
  int height = pos.h > 0 ? pos.h : WINDOW_HEIGHT;
  setResizable(false, false);
  juce::Desktop::getInstance().setGlobalScaleFactor((float)config.m_scale);
  setSize(width, height);
  addAndMakeVisible(mainInterpreter->componentGroup.get());
  auto *properties_ = config.properties();
  if (properties_->asBool("useOpenGL", true))
    setupGL(properties_->asBool("vsync", true));
}

void VeNoEditor::paint(juce::Graphics &g) {
  VENO_PROFILE_FUNCTION();
  g.fillAll(juce::Colour(0, 0, 0));
}
VeNoEditor::~VeNoEditor() {
  mainInterpreter.reset();
  VeNo::Core::Config::get().removeEditor(m_id);
  m_openGLContext.detach();
}
void VeNoEditor::resized() {
  VENO_PROFILE_FUNCTION();
  mainInterpreter->componentGroup->setBounds(0, 0, getWidth(), getHeight());
}
void VeNoEditor::setupGL(bool vsync) {
  DBGN("Use OpenGL Rendering");
  m_openGLContext.setRenderer(this);
  if (!vsync && !m_openGLContext.setSwapInterval(0))
    ERR("Cannot deactivate VSync");
  m_openGLContext.setContinuousRepainting(true);
  m_openGLContext.attachTo(*this);
  m_openGLContext.makeActive();
}
void VeNoEditor::newOpenGLContextCreated() {}
void VeNoEditor::renderOpenGL() {
  VENO_PROFILE_FUNCTION();
  auto &knobs =
      VeNo::Core::Instance::get(m_instanceId)->state.components.m_knobs;
  for (auto &knob : knobs)
    if (knob.second->isVisible())
      knob.second->repaint();
}
void VeNoEditor::openGLContextClosing() {}
