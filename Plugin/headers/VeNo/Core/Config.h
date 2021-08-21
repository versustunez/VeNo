#pragma once

#include <JuceHeader.h>
#include <VeNo/GUI/GuiLang/GUIHandler.h>
#include <VeNo/GUI/GuiLang/GUIInit.h>
#include <VeNo/GUI/Theme/LookAndFeel/LookHandler.h>
#include <VeNo/GUI/Theme/Theme.h>
#include <memory>
#include <unordered_map>

namespace VeNo::Core
{
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
    VeNo::GUI::Initializer guiInit;

    std::shared_ptr<VeNo::Theme::Theme> theme();
    VeNo::GUI::LookHandler* look;
    void setScale(double scale);
protected:
    Config();
    ~Config();
    void initConfig();
    static void createForPath (const std::string& path);
    std::mutex m_guard;
    static std::mutex createGuard;
    std::shared_ptr<juce::PropertiesFile> m_config;
    std::unordered_map<std::string, juce::AudioProcessorEditor*> m_editors;
    std::unordered_map<std::string, juce::AudioProcessor*> m_processors;
    bool m_isLayoutInit = false;
    bool m_isThemeInit = false;
    std::shared_ptr<VeNo::Theme::Theme> m_theme;
    void initTheme();
    void initLayout();
};
} // namespace VeNo::Core