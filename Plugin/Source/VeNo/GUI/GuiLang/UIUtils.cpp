#include <VUtils/Logging.h>
#include <VUtils/StringUtils.h>
#include <VeNo/Core/Config.h>
#include <VeNo/GUI/GuiLang/UIUtils.h>
#include <cmath>
#include <sstream>
#include <string>

namespace VeNo::GUI {

int UIUtils::getValue(VString value, int parentValue) {
  if (value.empty())
    return parentValue;
  bool isPercent = false;
  if (value[value.length() - 1] == '%') {
    isPercent = true;
    value = value.erase(value.length() - 1, 0);
  }
  auto newVal = VUtils::StringUtils::toNumber(value, 0);
  if (isPercent) {
    double val = newVal / 100.0;
    newVal = (int)std::round(parentValue * val);
  }
  return newVal;
}

int UIUtils::getOffsetFromParent(GUIParseItem *item,
                                 UIUtils::Direction direction) {
  int value = 0;
  // look on all previous heights
  for (auto *local : item->items) {
    value += direction == UIUtils::Direction::X ? local->pos.w : local->pos.h;
  }
  return value;
}
VString UIUtils::getParameterReplaced(UIParser *parser, VString &input) {
  if (input.find('{', 0) == VString::npos) {
    return input;
  }
  auto &parameters = parser->parameters();
  size_t first = 0;
  size_t last = 0;
  std::stringstream stream;
  for (size_t iterator = 0; iterator < input.length(); ++iterator) {
    if (input[iterator] == '{') {
      first = iterator;
      stream << input.substr(last, iterator - last);
    }
    if (input[iterator] == '}') {
      auto string = input.substr(first + 1, (iterator - first) - 1);
      if (parameters.find(string) != parameters.end())
        stream << parameters[string];
      last = iterator + 1;
    }
  }
  stream << input.substr(last, std::string::npos);
  return stream.str();
}
void UIUtils::setProperty(GUIParseItem *item, VString &name, VString &value) {
  auto *parent = item->parent;
  // clang-format off
  // SET POSITION STUFF
  if (name == "x"){
    item->pos.x = getValue(value, parent ? parent->pos.w : 0);
  } else if (name == "y"){
    item->pos.y = getValue(value, parent ? parent->pos.h : 0);
  } else if (name == "w" || name == "width"){
    item->pos.w = getValue(value, parent ? parent->pos.w : 0);
  } else if (name == "h" || name == "height"){
    item->pos.h = getValue(value, parent ? parent->pos.h : 0);
  } else if (name == "y-offset"){
    item->pos.y += getValue(value, item->parent ? item->parent->pos.w : 0);
  } else if (name == "x-offset") {
    item->pos.x += getValue(value, item->parent ? item->parent->pos.w : 0);
  } else if (name == "position") {
    auto val = value == "automatic" ? GUIDisplay::BLOCK : GUIDisplay::FIXED;
    item->display.x = val;
    item->display.y = val;
  } else if (name == "position-y") {
    item->display.y = value == "automatic" ? GUIDisplay::BLOCK : GUIDisplay::FIXED;
  } else if (name == "position-x") {
    item->display.x = value == "automatic" ? GUIDisplay::BLOCK : GUIDisplay::FIXED;
  } else if (name == "bind" && item->component != nullptr) {
    item->component->parameter = value;
  } else if (name == "bg" || name == "background" || name == "color") {
    setColor(item, value);
  } else if(name == "name") {
    item->name = value;
  } else {
    item->properties[name] = value;
  }
  // clang-format on
}
ImportItem UIUtils::getImportParameters(UIParser *parser, VString &import) {
  import = getParameterReplaced(parser, import);
  auto split = VUtils::StringUtils::split(import, ";");
  ImportItem importItem{split[0], {}};
  for (size_t i = 1; i < split.size(); ++i) {
    auto param = VUtils::StringUtils::split(split[i], "=", 1);
    if (param.size() > 1)
      importItem.params[param[0]] = param[1];
    else
      importItem.params[param[0]] = "true";
  }
  return importItem;
}
void UIUtils::setColor(GUIParseItem *item, VString &value) {
  item->colorComponent.hasColor = true;
  // preColor is a part we will fetch the Theme::Colors::index ;)
  auto color = Core::Config::get().theme()->getColorIndex(value);
  if (color != Theme::Colors::unknown) {
    item->colorComponent.isPreColor = true;
    item->colorComponent.preColor = color;
  } else {
    auto split = VUtils::StringUtils::split(value, ",");
    auto &colorComponent = item->colorComponent;
    if (split.size() != 4 && split.size() != 3) {
      colorComponent.hasColor = false;
      ERR("Wrong size: {} needed at least 3 and max 4", split.size());
      return;
    }
    colorComponent.isPreColor = false;
    for (size_t i = 0; i < split.size(); ++i)
      colorComponent.colors[i] = VUtils::StringUtils::toNumber(split[i], 255);
    colorComponent.color = {colorComponent.colors[0], colorComponent.colors[1],
                            colorComponent.colors[2], colorComponent.colors[3]};
  }
}
} // namespace VeNo::GUI