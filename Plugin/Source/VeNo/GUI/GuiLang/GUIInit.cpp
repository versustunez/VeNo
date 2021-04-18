#include <VUtils/Logging.h>
#include <VeNo/Core/Config.h>
#include <VeNo/GUI/GuiLang/GUIInit.h>

namespace VeNo::GUI
{
void Initalizer::createParser (std::string& name)
{
    if (! m_guiParser.contains (name))
        m_guiParser.emplace (name, std::make_shared<GUILangParser> (name, true));
}
void Initalizer::parseMain (std::string& name)
{
    auto& layoutPath = VeNo::Core::Config::get().layoutPath;
    auto mainFB = "Bin::MainGUI";
    if (name != mainFB && VUtils::FileHandler::fileExists (layoutPath + name))
        m_guiParser.emplace ("Main", std::make_shared<GUILangParser> (name, true));
    else
        m_guiParser.emplace ("Main", std::make_shared<GUILangParser> (Files::MainGui, false));

    auto& main = m_guiParser["Main"];
    main->parserName = mainFB;
    main->load();
    main->parse (false);
    parseAll();
}
void Initalizer::parseAll()
{
    auto& layoutPath = VeNo::Core::Config::get().layoutPath;
    for (auto& item : m_guiParser)
    {
        auto parser = item.second;
        if (parser == nullptr)
            continue;
        // Already Parsed!
        if (item.first == "Main")
            continue;
        auto fileName = layoutPath + parser->m_file;
        std::string content;
        if (parser->m_file.rfind ("Bin::", 0) != 0 && VUtils::FileHandler::fileExists (fileName))
            content = VUtils::FileHandler::readFile (fileName);
        else
            content = getBinaryContent (parser->m_file);
        parser->setContent (content);
        parser->parse (false);
    }
}
std::string Initalizer::getBinaryContent (const std::string& name)
{
    auto pos = name.rfind ("Bin::", 0);
    auto binaryName = name;
    if (pos == 0)
        binaryName = binaryName.substr (5, binaryName.length());
    for (auto& preDefinedBinarie : preDefinedBinaries)
    {
        if (binaryName == preDefinedBinarie.name)
            return preDefinedBinarie.data;
    }
    WARN ("UNKNOWN BINARY RETURN EMPTY!")
    return "";
}
GUIParseItem* Initalizer::get (const std::string& name)
{
    if (m_guiParser.contains (name) && m_guiParser[name] != nullptr)
        return &m_guiParser[name]->m_parsed;
    return nullptr;
}
Initalizer::~Initalizer()
{
    m_guiParser.clear();
}
} // namespace VeNo::GUI