#pragma once

#include <JuceHeader.h>
#include <VeNo/Core/PropertiesWrapper.h>
#include <VeNo/GUI/GuiLang/Init.h>
#include <VeNo/GUI/GuiLang/UIParser.h>
#include <VeNo/GUI/Theme/LookAndFeel/LookHandler.h>
#include <VeNo/GUI/Theme/Theme.h>
#include <VeNo/TypeDefs.h>
#include <unordered_map>

namespace VeNo::Core {
struct Config {
  static Config *m_instance;
  static Config &get();
  void registerEditor(std::string &id, juce::AudioProcessorEditor *editor);
  void removeEditor(std::string &id);
  void registerProcessor(std::string &id, juce::AudioProcessor *processor);
  void removeProcessor(std::string &id);
  double sampleRate = -1;
  double m_scale = 1.0;
  Properties *properties();
  std::string presetPath;
  std::string layoutPath;
  VeNo::GUI::Initializer guiInit;

  Ref<Theme::Theme> theme();
  Scope<VeNo::GUI::LookHandler> look{nullptr};
  void setScale(double scale);

protected:
  Config();
  ~Config();
  void initConfig();
  static void createForPath(const std::string &path);
  Mutex m_guard;
  static Mutex createGuard;
  Ref<Properties> m_config;
  std::unordered_map<std::string, juce::AudioProcessorEditor *> m_editors;
  std::unordered_map<std::string, juce::AudioProcessor *> m_processors;
  bool m_isLayoutInit = false;
  bool m_isThemeInit = false;
  Ref<Theme::Theme> m_theme;
  void initTheme();
  void initLayout();
};
} // namespace VeNo::Core