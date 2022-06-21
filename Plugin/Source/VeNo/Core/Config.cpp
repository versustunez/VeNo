#include <VUtils/Logging.h>
#include <VeNo/Core/Config.h>
#include <VeNo/GUI/Fonts/Fonts.h>
#include <VeNo/PluginEditor.h>

namespace VeNo::Core {
Mutex Config::createGuard{};
Config::Config() {
  using VUtils::FileHandler;
  initConfig();
  // Build on Top of JUCE LOGIC! Makes sure that All are on the same Path with
  // the Config ;)
  VUtils::FileHandler::m_currentDir =
      m_config->file().getParentDirectory().getFullPathName().toStdString();
  presetPath =
      FileHandler::resolve(m_config->asString("presetPath", "res://Presets/"));
  layoutPath =
      FileHandler::resolve(m_config->asString("layoutPath", "res://Layouts/"));
  createForPath(presetPath);
  createForPath(layoutPath);
  m_theme = CreateRef<Theme::Theme>(m_config);
  m_theme->init();
  DBGN("Preset Dir: {}", presetPath.c_str());
  DBGN("Layout Dir: {}", layoutPath.c_str());
}
Config::~Config() {
  DBGN("Deleting");
  GUI::Fonts::destroyAll();
  m_config->save();
  m_config.reset();
  m_theme = nullptr;
}
void Config::initConfig() {
  Guard lockGuard(m_guard);
  m_config = Properties::create("editor-config", "VeNo");
  DBGN("found @ {}", m_config->filename().c_str());
  m_scale = m_config->asDouble("editor.scale", 1.0);
}
void Config::registerEditor(std::string &id,
                            VeNoEditor *editor) {
  DBGN("ID: {}", id.c_str());
  initLayout();
  initTheme();
  m_editors[id] = editor;
}
void Config::removeEditor(std::string &id) {
  DBGN("ID: {}", id.c_str());
  m_editors.erase(id);
  if (m_editors.empty()) {
    GUI::Fonts::destroyAll();
    look = nullptr;
    m_isThemeInit = false;
  }
}
Config &Config::get() {
  Guard lockGuard(createGuard);
  static Config instance;
  return instance;
}
void Config::registerProcessor(std::string &id,
                               juce::AudioProcessor *processor) {
  DBGN("ID: {}", id.c_str());
  m_processors[id] = processor;
}
void Config::removeProcessor(std::string &id) {
  DBGN("ID: {}", id.c_str());
  m_processors.erase(id);
  // If all Processors are gone remove VeNo! this will also free-up memory from
  // WaveTables and other stuff! :P
  if (m_processors.empty()) {
    Guard lockGuard(m_guard);
    m_config->save();
    sampleRate = -1.0;
  }
}
Properties *Config::properties() { return m_config.get(); }
void Config::createForPath(const std::string &path) {
  if (!VUtils::FileHandler::createDirectoryIfNotExist(path))
    ERR("Oh cannot create Directories :(");
}

void Config::initLayout() {
  Guard lockGuard(m_guard);
  if (m_isLayoutInit)
    return;
  m_isLayoutInit = true;
  std::string file = m_config->asString("gui.main", "Bin::MainGui");
  guiInit.parseMain(file);
}
Ref<Theme::Theme> Config::theme() { return m_theme; }
void Config::initTheme() {
  Guard lockGuard(m_guard);
  if (m_isThemeInit)
    return;
  m_isThemeInit = true;
  look = CreateScope<GUI::LookHandler>();
}
void Config::setScale(double scale) {
  m_scale = scale;
  m_config->setValue("editor.scale", m_scale);
  for(auto& [idx,editor] : m_editors) {
    editor->setScaleFactor((float)scale);
    auto &pos = editor->mainInterpreter->componentGroup->position();
    int width = pos.w > 0 ? pos.w : WINDOW_WIDTH;
    int height = pos.h > 0 ? pos.h : WINDOW_HEIGHT;
    editor->setSize(width, height);
    editor->repaint();
  }

}
void Config::repaintEditors() {
  for(auto& [idx,editor] : m_editors) {
    editor->repaint();
  }
}
} // namespace VeNo::Core
