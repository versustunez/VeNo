#pragma once

#include <JuceHeader.h>
#include <VeNo/Core/Theme/Theme.h>
#include <VeNo/GUI/GuiLang/GUIHandler.h>
#include <VeNo/GUI/GuiLang/GUIInit.h>
#include <memory>
#include <vendor/tsl/robin_map.h>

namespace VeNo::Core
{
typedef VeNo::Theme::Theme Theme;
struct Config
{
    static Config* m_instance;
    static Config& get();
    void registerEditor (std::string& id, juce::AudioProcessorEditor* editor);
    void removeEditor (std::string& id);
    void registerProcessor (std::string& id, juce::AudioProcessor* processor);
    void removeProcessor (std::string& id);
    double sampleRate = -1;
    double m_scale = 1.0;
    juce::PropertiesFile* getProperties();
    std::string presetPath;
    std::string layoutPath;
    VeNo::GUI::Initalizer guiInit;

    void initLayout();

    std::shared_ptr<Theme> theme();

protected:
    Config();
    ~Config();
    void initConfig();
    static void createForPath (const std::string& path);
    std::mutex m_guard;
    static std::mutex createGuard;
    std::shared_ptr<juce::PropertiesFile> m_config;
    tsl::robin_map<std::string, juce::AudioProcessorEditor*> m_editors;
    tsl::robin_map<std::string, juce::AudioProcessor*> m_processors;
    bool m_isLayoutInit = false;
    std::shared_ptr<Theme> m_theme;
};
} // namespace VeNo::Core