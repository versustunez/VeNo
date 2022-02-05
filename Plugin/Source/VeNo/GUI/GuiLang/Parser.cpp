#include <VUtils/FileHandler.h>
#include <VUtils/Logging.h>
#include <VUtils/StringUtils.h>
#include <VeNo/Core/Config.h>
#include <VeNo/GUI/GuiLang/Parser.h>
#include <VeNo/GUI/GuiLang/Structs.h>
#include <VeNo/Utils/ProfileMacros.h>
#include <utility>

namespace VeNo::GUI {
GUILangParser::GUILangParser() = default;

GUIParseItem::~GUIParseItem() {
  for (auto &item : items)
    delete item;
  delete component;
}
std::string &GUIParseItem::operator[](const char *_name) {
  return this->properties[_name];
}

void GUILangParser::setContent(const std::string &content) {
  auto lines = VUtils::StringUtils::split(content, "\n");
  m_lines = lines;
}

void GUILangParser::load() {
  if (m_file.empty())
    return;
  if (!m_isFile) {
    setContent(m_file);
    m_file = parserName;
    return;
  }
  if (!VUtils::FileHandler::fileExists(m_file)) {
    ERR("Cannot find file %s", m_file.c_str());
    return;
  }
  auto content = VUtils::FileHandler::readFile(m_file);
  setContent(content);
}

void GUILangParser::parse(bool headOnly) {
  VENO_PROFILE_FUNCTION();
  if (rootItem == nullptr)
    rootItem = new GUIParseItem();
  auto *item = rootItem;
  int lineCount = 0;
  m_info.clear();
  for (auto &line : m_lines) {
    lineCount++;
    VUtils::StringUtils::trim(line);
    size_t lineSize = line.size();
    if (line.empty())
      continue;
    if (line.rfind("--", 0) == 0)
      continue;
    if (line.rfind('~', 0) == 0) {
      auto props = VUtils::StringUtils::split(line.substr(1, lineSize), ":");
      auto x = props[0];
      VUtils::StringUtils::toLower(x);
      if (props.size() == 2) {
        m_info[x] = VUtils::StringUtils::trimCopy(props[1]);
        continue;
      } else
        WARN("Property Line is corrupted! \"%s\" @ Line %d", x.c_str(),
            lineCount);
      continue;
    }
    if (headOnly) {
      DBGN("Finish Reading Head... @ Line %d", lineCount);
      break;
    }
    // Close Group or Component;
    if (line.rfind('}', 0) == 0) {
      // if parent is empty we are on the root! so everything is fine here
      if (m_isInvalid) {
        WARN("Skipping invalid Part %d - %d", crashedOnLine, lineCount);
        m_isInvalid = false;
        continue;
      }
      if (item->parent != nullptr) {
        item->parent->items.push_back(item);
        item = item->parent;
      } else {
        if (item->name != "root")
          ERR("Triggered a Wrong State... Found item that is not root @ Line "
              "%d",
              lineCount);
      }
      continue;
    }
    if (m_isInvalid)
      continue;
    if (line.back() == '{') {
      auto lastItem = item;
      item = new GUIParseItem();
      item->parent = lastItem;
      // if not defined then use full width first
      item->pos.w = item->parent->pos.w;
      item->pos.h = item->parent->pos.h;
      // Display Block means Stretch on Y :D
      if (lastItem->display.y == GUIDisplay::BLOCK) {
        item->pos.y = getOffsetFromParent(lastItem, false);
      }
      if (lastItem->display.x == GUIDisplay::BLOCK) {
        item->pos.x = getOffsetFromParent(lastItem, true);
      }
      if (lastItem->component == nullptr && line.rfind('#', 0) == 0)
        item->name = getParameterReplaced(
            VUtils::StringUtils::trimCopy(line.substr(1, lineSize - 2)));
      else if (line.rfind('@', 0) == 0) {
        // Component is available to have SubComponents but not Groups! ;)
        auto componentName =
            VUtils::StringUtils::trimCopy(line.substr(1, lineSize - 2));
        auto split = VUtils::StringUtils::split(componentName, "#", 1);
        item->component = new GUIComponent;
        item->component->name = split[0];
        if (split.size() == 2)
          item->name = split[1];
      } else {
        if (lastItem->component != nullptr) {
          crashedOnLine = lineCount;
          m_isInvalid = true;
        }
        ERR(R"(Invalid Operation "%s" @ Line %d ... Rollback)", line.c_str(),
            lineCount);
        auto temp = item->parent;
        delete item;
        item = temp;
        if (shouldCrash) {
          crashedOnLine = lineCount;
          break;
        }
      }
      continue;
    }
    if (line.rfind("@import", 0) == 0) {
      // Find Import...
      auto props = VUtils::StringUtils::split(line, "@import");
      if (props.size() != 2) {
        WARN("FOUND INVALID IMPORT AT: %d >> \"%s\"", lineCount, line.c_str());
        continue;
      }
      auto importName = VUtils::StringUtils::trimCopy(props[1]);
      auto import = getImportParameters(importName);
      if (import.name == parserName) {
        WARN("CANNOT IMPORT SAME FILE: %d >> \"%s\"", lineCount, line.c_str());
        continue;
      }
      auto &parser = Core::Config::get().guiInit.createParser(import.name);
      parser->rootItem = item;
      parser->parameters = import.params;
      parser->parse(headOnly);
      continue;
    }
    // split by : to get property name and value
    auto props = VUtils::StringUtils::split(line, ":", 1);
    if (props.size() < 2) {
      WARN("FOUND INVALID PROPERTY AT: %d >> \"%s\"", lineCount, line.c_str());
      if (shouldCrash) {
        crashedOnLine = lineCount;
        break;
      }
      continue;
    }
    auto name = VUtils::StringUtils::trimCopy(props[0]);
    auto value = getParameterReplaced(VUtils::StringUtils::trimCopy(props[1]));
    setProperty(item, name, value);
  }
  // Clear Parameters after Parsing
  parameters.clear();
}

GUILangParser::GUILangParser(std::string file, bool isFile)
    : m_file(std::move(file)), m_isFile(isFile),
      parserName(VUtils::FileHandler::getFileName(m_file)) {}
void GUILangParser::setProperty(
    GUIParseItem *item, std::string &name, std::string &value) {
  VENO_PROFILE_FUNCTION();
  if (name == "x")
    item->pos.x = getValue(value, item->parent ? item->parent->pos.w : 0);
  else if (name == "y")
    item->pos.y = getValue(value, item->parent ? item->parent->pos.h : 0);
  else if (name == "w" || name == "width")
    item->pos.w = getValue(value, item->parent ? item->parent->pos.w : 0);
  else if (name == "h" || name == "height")
    item->pos.h = getValue(value, item->parent ? item->parent->pos.h : 0);
  else if (name == "bind" && item->component != nullptr)
    item->component->parameter = value;
  else if (name == "position") {
    auto val = value == "automatic" ? GUIDisplay::BLOCK : GUIDisplay::FIXED;
    item->display.x = val;
    item->display.y = val;
  } else if (name == "position-y")
    item->display.y =
        value == "automatic" ? GUIDisplay::BLOCK : GUIDisplay::FIXED;
  else if (name == "position-x")
    item->display.x =
        value == "automatic" ? GUIDisplay::BLOCK : GUIDisplay::FIXED;
  else if (name == "name" && !value.empty())
    item->name = value;
  else if (name == "bg" || name == "background") {
    item->colorComponent.hasColor = true;
    if (Core::Config::get().theme()->colorExists(value)) {
      item->colorComponent.isPreColor = true;
      item->colorComponent.preColor = value;
    } else {
      auto split = VUtils::StringUtils::split(value, ",");
      if (split.size() != 4 && split.size() != 3) {
        item->colorComponent.hasColor = false;
        ERR("Wrong size: %d needed at least 3 and max 4", split.size());
        return;
      }
      item->colorComponent.isPreColor = false;
      for (size_t i = 0; i < split.size(); ++i)
        item->colorComponent.colors[i] =
            VUtils::StringUtils::toNumber(split[i], 255);
    }
  } else
    item->properties[name] = value;
}
ImportItem GUILangParser::getImportParameters(std::string &import) {
  VENO_PROFILE_FUNCTION();
  auto split = VUtils::StringUtils::split(import, ";");
  ImportItem importItem{split[0]};
  for (size_t i = 1; i < split.size(); ++i) {
    auto param = VUtils::StringUtils::split(split[i], "=", 1);
    if (param.size() > 1)
      importItem.params[param[0]] = param[1];
    else
      importItem.params[param[0]] = "true";
  }
  return importItem;
}
// Will Look if we have a % number or a normal and return the real value
int GUILangParser::getValue(std::string &value, int parentVal) {
  VENO_PROFILE_FUNCTION();
  bool isPercent = false;
  if (value[value.length() - 1] == '%') {
    isPercent = true;
    value = value.erase(value.length() - 1, 0);
  }
  auto newVal = VUtils::StringUtils::toNumber(value, 0);
  if (isPercent) {
    double val = newVal / 100.0;
    newVal = parentVal * val;
  }
  return newVal;
}
int GUILangParser::getOffsetFromParent(GUIParseItem *pItem, bool isX) {
  int value = 0;
  // look on all previous heights
  for (auto *item : pItem->items) {
    value += isX ? item->pos.w : item->pos.h;
  }
  return value;
}
GUILangParser::~GUILangParser() {
  // Because Double FREE Protection! -> The Root Parser only should trigger to
  // delete the item all others will delete with that!
  if (rootItem && rootItem->name == "root")
    delete rootItem;
}

std::string GUILangParser::getParameterReplaced(std::string input) {
  VENO_PROFILE_FUNCTION();
  auto words_begin = std::sregex_iterator(input.begin(), input.end(), regex);
  auto words_end = std::sregex_iterator();
  for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
    std::smatch match = *i;
    std::string replace;
    std::string key = match[1].str();
    if (parameters.find(key) != parameters.end())
      replace = parameters[key];
    input.replace(input.find(match[0]), match[0].str().size(), replace);
  }
  return input;
}

} // namespace VeNo::GUI
