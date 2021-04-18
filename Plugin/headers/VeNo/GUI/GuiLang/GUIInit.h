#pragma once

#include "GUIHandler.h"
#include "GuiInterpreter.h"
#include "../GUIFiles.h"
#include <vendor/tsl/robin_map.h>

namespace VeNo::GUI
{
struct preDefinedBinary
{
    std::string name;
    std::string data;
};
struct Initalizer
{
    Initalizer() = default;
    ~Initalizer();

    void parseMain (std::string& name);
    void parseAll();
    void createParser (std::string& name);

    GUIParseItem* get (const std::string& name);

protected:
    tsl::robin_map<std::string, std::shared_ptr<VeNo::GUI::GUILangParser>> m_guiParser;
    preDefinedBinary preDefinedBinaries[5] = {
        { "MainGUI", Files::MainGui },
        { "Oscillator", Files::OSC },
        { "LFO", "" },
        { "Filter", "" },
        { "LCD", "" }
    };
    std::string getBinaryContent (const std::string& name);
};
} // namespace VeNo::GUI