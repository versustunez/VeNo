#pragma once

#include <JuceHeader.h>
#include <VeNo/GUI/GuiLang/UIParser.h>
#include <VeNo/PluginEditor.h>
#include <VeNo/PluginProcessor.h>
#include <VeNo/TypeDefs.h>
#include <string>

using micro = std::chrono::duration<double, std::micro>;
using ms = std::chrono::duration<double, std::milli>;

namespace VeNo::Core {
class Config;
}
class VeNoEditor : public juce::AudioProcessorEditor,
                   public juce::OpenGLRenderer {
public:
  explicit VeNoEditor(VeNoProcessor &p, std::string id);
  ~VeNoEditor() override;

  void paint(juce::Graphics &) override;
  void resized() override;
  void setupGL(bool vsync);

  void newOpenGLContextCreated() override;
  void renderOpenGL() override;
  void openGLContextClosing() override;

  VeNo::Ref<VeNo::GUI::Interpreter> mainInterpreter{nullptr};

protected:
  std::string m_id;
  VeNo::InstanceID m_instanceId;
  juce::OpenGLContext m_openGLContext;
  VeNo::Core::Instance *m_instance;
  friend VeNo::Core::Config;
};
