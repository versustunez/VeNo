#pragma once

#include "../GUIFiles.h"
#include "GUIHandler.h"
#include "GuiInterpreter.h"

namespace VeNo::GUI
{
struct preDefinedBinary
{
    std::string name;
    std::string data;
};
struct Initializer
{
    Initializer() = default;
    ~Initializer();

    void parseMain (std::string& name);
    void parseAll();
    void createParser (std::string& name);

    GUIParseItem* get (const std::string& name);
    GUIParseItem* getOrCreate(const std::string& name);
protected:
    std::unordered_map<std::string, std::shared_ptr<VeNo::GUI::GUILangParser>> m_guiParser;
    preDefinedBinary preDefinedBinaries[5] = {
        { "Oscillator", Files::OSC },
        { "LFO", "" },
        { "Filter", "" },
        { "LCD", "" },
        { "WaveEditor", Files::WaveEditor }
    };
    std::string getBinaryContent (const std::string& name);
};
} // namespace VeNo::GUI