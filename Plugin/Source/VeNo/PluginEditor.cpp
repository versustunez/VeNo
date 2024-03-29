#include <VeNo/Core/Config.h>
#include <VeNo/Definitions.h>
#include <VeNo/PluginEditor.h>

VeNoEditor::VeNoEditor(VeNoProcessor &p, std::string id)
    : AudioProcessorEditor(&p),
      m_id(std::move(id)),
      m_instanceId(p.instance->id) {
  auto &config = VeNo::Core::Config::get();
  config.registerEditor(m_id, this);
  auto *instance = VeNo::Core::Instance::get(m_instanceId);
  m_instance = instance;
  mainInterpreter = instance->mainInterpreter;
  mainInterpreter->parseMain(config.guiInit.get("Main"));
  auto &pos = mainInterpreter->componentGroup->position();
  int width = pos.w > 0 ? pos.w : WINDOW_WIDTH;
  int height = pos.h > 0 ? pos.h : WINDOW_HEIGHT;
  setResizable(false, false);
  setSize(width, height);
  auto *properties_ = config.properties();
  if (properties_->asBool("useOpenGL", true))
    setupGL(properties_->asBool("vsync", true));
  instance->state.actionRegistry =
      VeNo::CreateScope<VeNo::GUI::ActionRegistry>(m_instanceId);
  addAndMakeVisible(mainInterpreter->componentGroup.get());
}

void VeNoEditor::paint(juce::Graphics &g) { g.fillAll(juce::Colour(0, 0, 0)); }
VeNoEditor::~VeNoEditor() {
  auto *instance = VeNo::Core::Instance::get(m_instanceId);
  instance->state.actionRegistry.reset(nullptr);
  mainInterpreter->removeUI();
  mainInterpreter.reset();
  VeNo::Core::Config::get().removeEditor(m_id);
  m_openGLContext.detach();
}
void VeNoEditor::resized() {
  mainInterpreter->componentGroup->setBounds(0, 0, getWidth(), getHeight());
}
void VeNoEditor::setupGL(bool vsync) {
  DBGN("Use OpenGL Rendering");
  m_openGLContext.setRenderer(this);
  if (!vsync && !m_openGLContext.setSwapInterval(0))
    ERR("Cannot deactivate VSync");
  m_openGLContext.setContinuousRepainting(false);
  m_openGLContext.setComponentPaintingEnabled(true);
  m_openGLContext.attachTo(*this);
  m_openGLContext.makeActive();
}
void VeNoEditor::newOpenGLContextCreated() {}
void VeNoEditor::renderOpenGL() {}
void VeNoEditor::openGLContextClosing() {}
