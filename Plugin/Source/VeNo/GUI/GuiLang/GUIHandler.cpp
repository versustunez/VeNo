#include <VUtils/FileHandler.h>
#include <VUtils/Logging.h>
#include <VUtils/StringUtils.h>
#include <VeNo/Core/Config.h>
#include <VeNo/GUI/GuiLang/GUIHandler.h>
#include <utility>

namespace VeNo::GUI
{
GUILangParser::GUILangParser() = default;

GUIParseItem::~GUIParseItem()
{
    for (auto& item : items)
        delete item;
    delete component;
}

void GUILangParser::setContent (const std::string& content)
{
    auto lines = VUtils::StringUtils::split (content, "\n");
    this->m_lines = lines;
}

void GUILangParser::load()
{
    if (m_file.empty())
        return;
    if (! m_isFile)
    {
        setContent (m_file);
        m_file = parserName;
        return;
    }
    if (! VUtils::FileHandler::fileExists (m_file))
    {
        ERR ("Cannot find file %s", m_file.c_str());
        return;
    }
    auto content = VUtils::FileHandler::readFile (m_file);
    setContent (content);
}

void GUILangParser::parse (bool headOnly)
{
#ifdef DEBUG
    DBGN ("Start Parsing: %d lines of GUI File %s", m_lines.size(), m_file.c_str())
#endif
    m_parsed.name = "root";
    auto* item = &m_parsed;
    int lineCount = 0;
    m_info.clear();
    for (auto& line : m_lines)
    {
        lineCount++;
        VUtils::StringUtils::trim (line);
        size_t lineSize = line.size();
        if (line.empty())
            continue;
        if (line.rfind ("--", 0) == 0)
            continue;
        if (line.rfind ('~', 0) == 0)
        {
            auto props = VUtils::StringUtils::split (line.substr (1, lineSize), ":");
            auto x = props[0];
            VUtils::StringUtils::toLower (x);
            if (props.size() == 2)
            {
                m_info[x] = VUtils::StringUtils::trimCopy (props[1]);
                continue;
            }
            else
                WARN ("Property Line is corrupted! \"%s\" @ Line %d", x.c_str(), lineCount)
            continue;
        }
        if (headOnly)
        {
            DBGN ("Finish Reading Head... @ Line %d", lineCount)
            break;
        }
        // Close Group or Component;
        if (line.rfind ('}', 0) == 0)
        {
            // if parent is empty we are on the root! so everything is fine here
            if (m_isInvalid)
            {
                DBGN ("Skipping invalid Part %d - %d", crashedOnLine, lineCount)
                m_isInvalid = false;
                continue;
            }
            if (item->parent != nullptr)
            {
                item->parent->items.push_back (item);
                item = item->parent;
            }
            else
            {
                if (item->name != "root")
                {
                    ERR ("Triggerd a Wrong State... Found item that is not root @ Line %d", lineCount)
                }
            }
            continue;
        }
        if (m_isInvalid)
            continue;
        if (line.back() == '{')
        {
            auto lastItem = item;
            item = new GUIParseItem();
            item->parent = lastItem;
            if (lastItem->component == nullptr && line.rfind ('#', 0) == 0)
                item->name = VUtils::StringUtils::trimCopy (line.substr (1, lineSize - 2));
            else if (line.rfind ('@', 0) == 0)
            {
                // Component is available to have SubComponents but not Groups! ;)
                auto componentName = VUtils::StringUtils::trimCopy (line.substr (1, lineSize - 2));
                auto split = VUtils::StringUtils::split (componentName, "#");
                item->component = new GUIComponent;
                item->component->name = split[0];
                if (split.size() == 2)
                    item->name = split[1];
            }
            else
            {
                if (lastItem->component != nullptr)
                {
                    crashedOnLine = lineCount;
                    m_isInvalid = true;
                }
                ERR ("Invalid Operation \"%s\" @ Line %d ... Rollback", line.c_str(), lineCount)
                auto temp = item->parent;
                delete item;
                item = temp;
                if (shouldCrash)
                {
                    crashedOnLine = lineCount;
                    break;
                }
            }
            continue;
        }
        if (line.rfind ("@import", 0) == 0)
        {
            // Find Import...
            auto props = VUtils::StringUtils::split (line, "@import");
            if (props.size() != 2)
            {
                WARN ("FOUND INVALID IMPORT AT: %d >> \"%s\"", lineCount, line.c_str());
                continue;
            }
            auto importName = VUtils::StringUtils::trimCopy (props[1]);
            if (importName == parserName)
            {
                WARN ("CANNOT IMPORT SAME FILE: %d >> \"%s\"", lineCount, line.c_str());
                continue;
            }
            auto import = getImportParameters (importName);
            VeNo::Core::Config::get().guiInit.createParser (import.name);
            item->imports.push_back (import);
            continue;
        }
        // split by : to get property name and value
        auto props = VUtils::StringUtils::split (line, ":");
        if (props.size() < 2)
        {
            WARN ("FOUND INVALID PROPERTY AT: %d >> \"%s\"", lineCount, line.c_str());
            if (shouldCrash)
            {
                crashedOnLine = lineCount;
                break;
            }
            continue;
        }
        auto name = VUtils::StringUtils::trimCopy (props[0]);
        auto value = VUtils::StringUtils::trimCopy (props[1]);
        setProperty (item, name, value);
    }
    DBGN ("Finish Parsing...")
}

GUILangParser::GUILangParser (std::string file, bool isFile) : m_file (std::move (file)),
                                                               m_isFile (isFile),
                                                               parserName (VUtils::FileHandler::getFileName (m_file))
{
}
void GUILangParser::setProperty (GUIParseItem* item, std::string& name, std::string& value)
{
    if (name == "x")
        item->pos.x = getPercentValueIfPercent(value, item->parent ? item->parent->pos.x : 0);
    else if (name == "y")
        item->pos.y = getPercentValueIfPercent(value, item->parent ? item->parent->pos.y : 0);
    else if (name == "w")
        item->pos.w = getPercentValueIfPercent(value, item->parent ? item->parent->pos.w : 0);
    else if (name == "h")
        item->pos.h = getPercentValueIfPercent(value, item->parent ? item->parent->pos.h : 0);
    else if (name == "bind" && item->component != nullptr)
        item->component->parameter = value;
    else if (name == "bg")
    {
        item->colorComponent.hasColor = true;
        if (VeNo::Core::Config::get().theme()->colorExists (value))
        {
            item->colorComponent.isPreColor = true;
            item->colorComponent.preColor = value;
        }
        else
        {
            auto split = VUtils::StringUtils::split (value, ",");
            if (split.size() != 4 && split.size() != 3)
            {
                item->colorComponent.hasColor = false;
                ERR ("Wrong size: %d needed at least 3 and max 4", split.size());
                return;
            }
            item->colorComponent.isPreColor = false;
            for (size_t i = 0; i < split.size(); ++i)
                item->colorComponent.colors[i] = VUtils::StringUtils::toNumber (split[i], 255);
        }
    }
    else
        item->properties[name] = value;
}
ImportItem GUILangParser::getImportParameters (std::string& import)
{
    auto split = VUtils::StringUtils::split (import, ";");
    ImportItem importItem { split[0] };
    for (size_t i = 1; i < split.size(); ++i)
    {
        auto param = VUtils::StringUtils::split (split[i], "=");
        if (param.size() > 1)
            importItem.params[param[0]] = param[1];
        else
            importItem.params[param[0]] = "true";
    }
#ifdef DEBUG
    DBGN ("Add Import: %s", import.c_str());
#endif
    return importItem;
}
int GUILangParser::getPercentValueIfPercent (std::string& value, int parentVal)
{
    bool isPercent = false;
    if (value[value.length() - 1] == '%')
    {
        isPercent = true;
        value = value.erase (value.length() - 1, 0);
    }
    auto newVal = VUtils::StringUtils::toNumber (value, 0);
    if (isPercent)
    {
        double val = newVal / 100.0;
        newVal = parentVal * val;
    }
    return newVal;
}
GUILangParser::~GUILangParser() = default;

} // namespace VeNo::GUI
