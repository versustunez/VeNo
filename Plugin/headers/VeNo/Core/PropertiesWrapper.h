#pragma once
#include <JuceHeader.h>
#include <VUtils/FileHandler.h>
#include <VUtils/Logging.h>
#include <VUtils/StringUtils.h>
#include <VeNo/TypeDefs.h>

namespace VeNo::Core {
class Properties {
public:
  static Scope<Properties> create(const char *file, const char *folder,
                                  const std::string &type = "xml") {
    juce::PropertiesFile::Options options;
    options.applicationName = file;
    options.folderName = folder;
    options.osxLibrarySubFolder = "Application";
    options.filenameSuffix = type;
    return CreateScope<Properties>(options);
  }
  explicit Properties(const juce::PropertiesFile::Options &_options) {
    m_file = _options.getDefaultFile();
    if (m_file.exists()) {
      root = juce::XmlDocument::parse(m_file);
    } else {
      m_file.create();
      root = CreateScope<juce::XmlElement>("properties");
    }
  }

  ~Properties() {
    DBGN("Close PropertyFile: %s", filename().c_str());
    save();
    root.reset(nullptr);
  }

  void save() {
    root->sortChildElements(*this, true);
    VUtils::FileHandler::writeFile(m_file.getFullPathName().toStdString(),
                                   root->toString().toStdString());
  }

  static int compareElements(const juce::XmlElement *first,
                             const juce::XmlElement *second) {
    auto firstIsProperty = first->getTagName().equalsIgnoreCase("property");
    auto secondIsProperty = second->getTagName().equalsIgnoreCase("property");
    if (firstIsProperty && secondIsProperty)
      return first->getStringAttribute("key").compare(
          second->getStringAttribute("key"));
    if (firstIsProperty)
      return -1;
    if (secondIsProperty)
      return 1;
    return first->getTagName().compare(second->getTagName());
  }

  double asDouble(const std::string &key, double fallback) {
    auto *el = getString<double>(key, fallback);
    return el ? el->getDoubleValue() != 0 : fallback;
  }

  bool asBool(const std::string &key, bool fallback) {
    auto *el = getString<int>(key, fallback ? 1 : 0);
    return el ? el->getIntValue() != 0 : fallback;
  }

  int asInt(const std::string &key, int fallback) {
    auto *el = getString<int>(key, fallback);
    return el ? el->getIntValue() : fallback;
  }

  std::string asString(const std::string &key, const std::string &fallback) {
    auto *el = getString<std::string>(key, fallback);
    return el ? el->toStdString() : fallback;
  }

  juce::File &file() { return m_file; }

  std::string filename() { return m_file.getFullPathName().toStdString(); }

  template <class Type> void setValue(const std::string &property, Type value) {
    juce::XmlElement *element = get<Type>(property, value);
    element->setAttribute("value", juce::String(value));
  }

protected:
  template <class T>
  const juce::String *getString(const std::string &property, T defaultValue) {
    juce::XmlElement *element = get<T>(property, defaultValue);
    return &element->getStringAttribute("value");
  }

  template <class T>
  juce::XmlElement *get(const std::string &property, T defaultValue) {
    // logic here ;)
    juce::XmlElement *current = root.get();
    auto split = VUtils::StringUtils::split(property, ".");
    auto lastItem = split.back();
    split.pop_back();
    for (auto &item : split) {
      auto *last = current;
      current = current->getChildByName(item);
      if (!current) {
        current = new juce::XmlElement(juce::String(item));
        last->addChildElement(current);
      }
    }

    auto *outputItem = current->getChildByAttribute("key", lastItem);
    if (!outputItem) {
      outputItem = new juce::XmlElement("property");
      outputItem->setAttribute("key", lastItem);
      outputItem->setAttribute("value", juce::String(defaultValue));
      current->addChildElement(outputItem);
    }
    return outputItem;
  }

  juce::File m_file;
  Scope<juce::XmlElement> root{nullptr};
};
} // namespace VeNo::Core