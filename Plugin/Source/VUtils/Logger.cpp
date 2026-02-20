#include "VUtils/FileHandler.h"
#include "VUtils/LogDefines.h"

#include <JuceHeader.h>
#include <VUtils/Logger.h>

namespace VUtils {

void Logger::create(const std::string &output, PrintType type) {
  if (juce::JUCEApplicationBase::isStandaloneApp())
    std::cout << fmt::format("\033[1;{}m[{}] >> {}\033[0m\n", getColor(type), getType(type),
               output);
}
void Logger::create(const std::string &output, const char *file,
                    const char *func, PrintType type) {
  if (juce::JUCEApplicationBase::isStandaloneApp())
    std::cout << fmt::format("\033[1;{}m[{}][{}] >> {}\033[0m\n", getColor(type),
               getType(type), getPlace(file, func), output);
}

int Logger::getColor(PrintType type) {
  switch (type) {
  case PrintType::Error: return ERROR_COLOR;
  case PrintType::Debug: return DEBUG_COLOR;
  case PrintType::Warn: return WARN_COLOR;
  case PrintType::Log: return INFO_COLOR;
  }
  return 35;
}

std::string Logger::getType(PrintType type) {
  switch (type) {
  case PrintType::Error: return ERROR_KEY;
  case PrintType::Debug: return DEBUG_KEY;
  case PrintType::Warn: return WARN_KEY;
  case PrintType::Log: return INFO_KEY;
  }
  return {};
}

std::string Logger::getPlace(const char *file, const char *name) {
  return VUtils::FileHandler::getFileName(file) + "::" + name;
}
} // namespace VUtils
