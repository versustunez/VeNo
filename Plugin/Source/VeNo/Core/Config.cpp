#include <VUtils/Logging.h>
#include <VeNo/Core/Config.h>
#include <VeNo/GUI/Fonts/Fonts.h>
#include <VeNo/Sound/Generation/WaveTables/Creator/Generator.h>

namespace VeNo::Core
{
Config* Config::m_instance = nullptr;
std::mutex Config::createGuard {};
Config::Config()
{
    initConfig();
    // Build on Top of JUCE LOGIC! Makes sure that All are on the same Path with the Config ;)
    auto homeDir = m_config->getFile().getParentDirectory().getFullPathName().toStdString();
    presetPath = m_config->getValue ("presetPath", homeDir + "/Presets/").toStdString();
    layoutPath = m_config->getValue ("layoutPath", homeDir + "/Layouts/").toStdString();
    createForPath (presetPath);
    createForPath (layoutPath);
    m_theme = std::make_shared<VeNo::Theme::Theme> (m_config);
    m_theme->init();
    DBGN ("Preset Dir: %s", presetPath.c_str())
    DBGN ("Layout Dir: %s", layoutPath.c_str())
}
Config::~Config()
{
    DBGN ("Deleting")
    VeNo::GUI::Fonts::destroyAll();
    m_config->save();
    m_config.reset();
    m_theme = nullptr;
}
void Config::initConfig()
{
    std::lock_guard<std::mutex> lockGuard (m_guard);
    juce::PropertiesFile::Options options;
    options.applicationName = "editor-config";
    options.folderName = "VeNo";
    options.filenameSuffix = "xml";
    m_config = std::make_unique<juce::PropertiesFile> (options);
    DBGN ("found @ %s", m_config->getFile().getFullPathName().toStdString().c_str())
    m_scale = m_config->getDoubleValue ("editor_scale", 1.0);
}
void Config::registerEditor (std::string& id, juce::AudioProcessorEditor* editor)
{
    DBGN ("ID: %s", id.c_str())
    initLayout();
    initTheme();
    m_editors[id] = editor;
}
void Config::removeEditor (std::string& id)
{
    DBGN ("ID: %s", id.c_str())
    m_editors.erase (id);
    if (m_editors.empty())
    {
        VeNo::GUI::Fonts::destroyAll();
        delete look;
        m_isThemeInit = false;
    }
}
Config& Config::get()
{
    std::lock_guard<std::mutex> lockGuard (createGuard);
    static Config instance;
    return instance;
}
void Config::registerProcessor (std::string& id, juce::AudioProcessor* processor)
{
    DBGN ("ID: %s", id.c_str())
    m_processors[id] = processor;
}
void Config::removeProcessor (std::string& id)
{
    DBGN ("ID: %s", id.c_str())
    m_processors.erase (id);
    // If all Processors are gone remove VeNo! this will also free-up memory from WaveTables and other stuff! :P
    if (m_processors.empty())
    {
        std::lock_guard<std::mutex> lockGuard (m_guard);
        VeNo::Audio::Waves::WaveTableGenerator::getInstance().cleanTables();
        m_config->save();
        sampleRate = -1.0;
    }
}
juce::PropertiesFile* Config::getProperties()
{
    return m_config.get();
}
void Config::createForPath (const std::string& path)
{
    if (! VUtils::FileHandler::createDirectoryIfNotExist (path))
        ERR ("Ohhh cannot create Directories :(")
}

void Config::initLayout()
{
    std::lock_guard<std::mutex> lockGuard (m_guard);
    if (m_isLayoutInit)
        return;
    m_isLayoutInit = true;
    std::string file = m_config->getValue ("layoutFile", "Bin::MainGUI").toStdString();
    guiInit.parseMain (file);
}
std::shared_ptr<VeNo::Theme::Theme> Config::theme()
{
    return m_theme;
}
void Config::initTheme()
{
    std::lock_guard<std::mutex> lockGuard (m_guard);
    if (m_isThemeInit)
        return;
    m_isThemeInit = true;
    look = new VeNo::GUI::LookHandler();
}
} // namespace VeNo::Core
