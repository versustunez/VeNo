#include <JuceHeader.h>
#include <VUtils/FileHandler.h>
#include <VUtils/Logging.h>
#include <fstream>
#include <vector>
namespace VUtils {
std::string FileHandler::m_currentDir;
bool FileHandler::fileExists(const std::string &fileName) {
  return juce::File(fileName).existsAsFile();
}
bool FileHandler::createFile(const std::string &fileName) {
  return juce::File(fileName).create();
}

std::string FileHandler::readFile(const std::string &fileName) {
  auto file = juce::File(fileName);
  if (!file.existsAsFile())
    return "";
  return file.loadFileAsString().toStdString();
}

bool FileHandler::writeFile(const std::string &fileName,
                            const std::string &content) {
  if (!fileExists(fileName) && !createFile(fileName))
    return false;
  try {
    std::ofstream ofs(fileName);
    ofs << content;
    ofs.close();
  } catch (std::exception &e) {
    ERR("Save Failed: {}", e.what());
    return false;
  }
  return true;
}
bool FileHandler::isDirectory(const std::string &fileName) {
  auto file = juce::File(fileName);
  return file.isDirectory();
}

std::string FileHandler::getExtension(const std::string &fileName) {
  auto file = juce::File(fileName);
  return file.getFileExtension().toStdString();
}

std::string FileHandler::getFileName(const std::basic_string<char> &name) {
  if (name.find("Bin::") == 0)
    return name;
  auto pos = name.find('/');
  return pos == 0 || pos == 1
             ? juce::File(name).getFileNameWithoutExtension().toStdString()
             : name;
}

bool FileHandler::createDirectoryIfNotExist(
    const std::basic_string<char> &fileName) {
  auto file = juce::File(fileName);
  if (!file.exists())
    return file.createDirectory().wasOk();
  return true;
}
bool FileHandler::createParentDirectories(
    const std::basic_string<char> &fileName) {
  auto file = juce::File(fileName).getParentDirectory();
  if (!file.exists())
    return file.createDirectory().wasOk();
  return true;
}

std::string FileHandler::resolve(const std::basic_string<char> &path) {
  std::string file = path;
  if (file.find("res://") == 0)
    file.replace(0, 5, VUtils::FileHandler::m_currentDir);
  return file;
}

} // namespace VUtils
