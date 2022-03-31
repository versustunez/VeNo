#pragma once

#include <JuceHeader.h>
#include <VeNo/GUI/GuiLang/UIParser.h>
#include <VeNo/PluginEditor.h>
#include <VeNo/PluginProcessor.h>
#include <string>

using micro = std::chrono::duration<double, std::micro>;
using ms = std::chrono::duration<double, std::milli>;

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

protected:
  std::string m_id;
  InstanceID m_instanceId;
  VeNo::Ref<VeNo::GUI::Interpreter> mainInterpreter{nullptr};
  juce::OpenGLContext m_openGLContext;
};
