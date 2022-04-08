#pragma once

#include <VeNo/GUI/Theme/Theme.h>
#include <VeNo/TypeDefs.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace VeNo::GUI {
enum GUIDisplay { FIXED = 0, BLOCK };

struct Position {
  int x, y, w, h;
};

struct Display {
  GUIDisplay x{}, y{};
};

struct GUIColorComponent {
  bool hasColor{false};
  bool isPreColor = false;
  Theme::Colors preColor{Theme::Colors::unknown};
  juce::Colour color;
  u8 colors[4] = {0, 0, 0, 255};
};

struct GUIComponent {
  VString name;
  VString parameter;
};

struct ImportItem {
  VString name;
  Map<std::string, std::string> params;
};

struct GUIParseItem {
  ~GUIParseItem() {
    for (auto &item : items)
      delete item;
    delete component;
  };
  GUIParseItem() = default;
  GUIParseItem(GUIParseItem &) = delete;
  VString name;
  Map<VString, VString> properties;
  Vector<GUIParseItem *> items; // we delete everything here ;)
  GUIParseItem *parent = nullptr;
  GUIComponent *component = nullptr;
  GUIColorComponent colorComponent;
  Display display{};
  Position pos = {0, 0, 1, 1};
  bool has(const char *m_name) {
    return properties.find(m_name) != properties.end();
  }
  VString &operator[](const char *_name) { return this->properties[_name]; }
};
} // namespace VeNo::GUI