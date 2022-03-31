#include <VeNo/Core/Config.h>
#include <VeNo/Utils/ProfileMacros.h>

#include <VUtils/StringUtils.h>
#include <VeNo/GUI/GuiLang/UIParser.h>
#include <VeNo/GUI/GuiLang/UIUtils.h>
#include <fstream>

namespace VeNo::GUI {

void UIParser::setContent(const VString &content) {
  m_isFile = false;
  m_lines = VUtils::StringUtils::split(content, "\n");
}

void UIParser::loadFile(const VString &filePath) {
  VENO_PROFILE_FUNCTION();
  VString line;
  std::ifstream fileToRead(filePath);
  if (fileToRead.is_open()) {
    while (getline(fileToRead, line))
      m_lines.push_back(line);
    m_isFile = true;
    fileToRead.close();
  } else {
    fprintf(stderr, "Failed to read file: %s", filePath.c_str());
  }
}

// NOLINTNEXTLINE
void UIParser::parse() {
  VENO_PROFILE_FUNCTION();
  if (m_lines.empty())
    return;
  if (m_rootItem == nullptr)
    m_rootItem = new GUIParseItem();
  m_info.clear();
  auto *item = m_rootItem;
  for (size_t i = 0; i < m_lines.size(); ++i) {
    auto &line = m_lines[i];
    VUtils::StringUtils::trim(line);
    if (line.empty() || line.find("--", 0) == 0)
      continue;
    if (line.rfind('~', 0) == 0) {
      parseInfo(line);
      continue;
    }
    auto loop = getLoop(line, i);
    if (loop.times > -1) {
      loop.stackSize = m_stack.size();
      loop.itVar = "it" + std::to_string(m_loopStack.size());
      m_loopStack.push(loop);
      m_parameters[loop.itVar] = std::to_string(loop.done+1);
      continue;
    } else if (line.rfind(')') == 0) {
      if (m_loopStack.empty() ||
          m_loopStack.top().stackSize != m_stack.size()) {
        ERR("Closing Loop with broken Stack Size... you missing a closing }");
        if (m_shouldCrash) {
          m_erroredLine = i;
          break;
        }
        continue;
      }
      auto &top = m_loopStack.top();
      top.done++;
      if (top.times > top.done) {
        i = top.beginningLine;
        m_parameters[top.itVar] = std::to_string(top.done+1);
        continue;
      }
      m_loopStack.pop();
    } else if (line.rfind("@import", 0) == 0) {
      importLine(item, line);
    } else if (line.rfind('@', 0) == 0) {
      item = createComponent(item, line);
    } else if (line.rfind('#', 0) == 0) {
      item = createGroup(item, line);
    } else if (line.rfind('}', 0) == 0) {
      if (m_stack.empty()) {
        ERR("TRYING TO POP MORE THEN AVAILABLE");
        if (m_shouldCrash) {
          m_erroredLine = i;
          break;
        }
        continue;
      }
      if (item->parent != nullptr) {
        item->parent->items.push_back(item);
        item = item->parent;
      }
      m_stack.pop();
    } else {
      auto props = VUtils::StringUtils::split(line, ":", 1);
      if (props.size() != 2) {
        if (m_shouldCrash) {
          ERR("Invalid Property Size... Doesnt Match 2");
          m_erroredLine = i;
          break;
        }
        continue;
      }
      auto name = VUtils::StringUtils::trimCopy(props[0]);
      auto value = VUtils::StringUtils::trimCopy(props[1]);
      value = UIUtils::getParameterReplaced(this, value);
      UIUtils::setProperty(item, name, value);
    }
  }
  m_parameters.clear();
}

GUIParseItem *UIParser::createNew(GUIParseItem *previous) {
  VENO_PROFILE_FUNCTION();
  auto item = new GUIParseItem();
  item->parent = previous;
  item->pos.w = previous->pos.w;
  item->pos.h = previous->pos.h;
  if (previous->display.y == GUIDisplay::BLOCK)
    item->pos.y = UIUtils::getOffsetFromParent(previous, UIUtils::Direction::Y);
  if (previous->display.x == GUIDisplay::BLOCK)
    item->pos.x = UIUtils::getOffsetFromParent(previous, UIUtils::Direction::X);
  m_stack.push(item);
  return item;
}

GUIParseItem *UIParser::createGroup(GUIParseItem *previous, VString &line) {
  auto item = createNew(previous);
  auto name = VUtils::StringUtils::trimCopy(line.substr(1, line.size() - 2));
  item->name = UIUtils::getParameterReplaced(this, name);
  return item;
}

GUIParseItem *UIParser::createComponent(GUIParseItem *previous, VString &line) {
  auto item = createNew(previous);
  auto componentName =
      VUtils::StringUtils::trimCopy(line.substr(1, line.size() - 2));
  auto split = VUtils::StringUtils::split(componentName, "#", 1);
  item->component = new GUIComponent;
  item->component->name = split[0];
  if (split.size() == 2) {
    item->name = UIUtils::getParameterReplaced(this, split[1]);
  }
  return item;
}

void UIParser::parseInfo(VString &line) {
  auto props = VUtils::StringUtils::split(line.substr(1, line.size()), ":");
  auto x = props[0];
  VUtils::StringUtils::toLower(x);
  if (props.size() == 2) {
    m_info[x] = VUtils::StringUtils::trimCopy(props[1]);
  }
}

// NOLINTNEXTLINE
void UIParser::importLine(GUIParseItem *previous, VString &line) {
  VENO_PROFILE_FUNCTION();
  auto props = VUtils::StringUtils::split(line, "@import");
  if (props.size() != 2) {
    // TRIGGER WARNING ;)
    WARN("FOUND INVALID IMPORT: \"{}\"", line.c_str());
    return;
  }
  auto importString = VUtils::StringUtils::trimCopy(props[1]);
  auto import = UIUtils::getImportParameters(this, importString);
  if (import.name == m_name) {
    WARN("CANNOT IMPORT SAME FILE >> \"{}\"", line.c_str());
    return;
  }
  // yeah this logic is missing ;) we need to have a createParser thingy :D
  auto &parser = Core::Config::get().guiInit.createParser(import.name);
  parser->m_rootItem = previous;
  parser->m_parameters = import.params;
  parser->parse();
}
UIParserLoop UIParser::getLoop(VString &line, size_t lineNumber) {
  VENO_PROFILE_FUNCTION();
  UIParserLoop loop;
  loop.beginningLine = lineNumber;
  auto startPos = line.rfind('[');
  auto endPos = line.rfind("](");
  if (startPos == VString::npos || endPos == VString::npos) {
    return loop;
  }
  // how often?
  auto timesString = line.substr(startPos + 1, endPos - 1);
  loop.times = VUtils::StringUtils::toNumber(timesString, -1);
  return loop;
}

} // namespace VeNo::GUI