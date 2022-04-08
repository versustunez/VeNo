#include "VUtils/FileHandler.h"
#include "VUtils/LogDefines.h"

#include <VUtils/Logger.h>

namespace VUtils {

void Logger::create(const std::string &output, PrintType type) {
  fmt::print("\033[1;{}m[{}] >> {}\033[0m\n", getColor(type), getType(type),
             output);
}
void Logger::create(const std::string &output, const char *file,
                    const char *func, PrintType type) {
  fmt::print("\033[1;{}m[{}][{}] >> {}\033[0m\n", getColor(type), getType(type),
             getPlace(file, func), output);
}

int Logger::getColor(PrintType type) {
  switch (type) {
  case PrintType::ERROR: return ERROR_COLOR;
  case PrintType::DBG: return DEBUG_COLOR;
  case PrintType::WARN: return WARN_COLOR;
  case PrintType::LOG: return INFO_COLOR;
  }
  return 35;
}

std::string Logger::getType(PrintType type) {
  switch (type) {
  case PrintType::ERROR: return ERROR_KEY;
  case PrintType::DBG: return DEBUG_KEY;
  case PrintType::WARN: return WARN_KEY;
  case PrintType::LOG: return INFO_KEY;
  }
  return {};
}

std::string Logger::getPlace(const char *file, const char *name) {
  return VUtils::FileHandler::getFileName(file) + "::" + name;
}
} // namespace VUtils