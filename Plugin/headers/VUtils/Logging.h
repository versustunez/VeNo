#pragma once

#include <VUtils/FileHandler.h>
#include <string>

#ifdef DEBUG
#define DEBUGLOG(message, mod) Logging::debugMod(message, mod)
#define DBGN(...) Logging::debug(true, __FILE__, __FUNCTION__, __VA_ARGS__)
#else
#define DEBUGLOG(message, mod)
#define DBGN(...)
#define DBGWN(...)
#endif
#define ERR(...) Logging::error(true, __FILE__, __FUNCTION__, __VA_ARGS__)
#define LOG(...) Logging::log(true, __FILE__, __FUNCTION__, __VA_ARGS__)
#define WARN(...) Logging::warn(true, __FILE__, __FUNCTION__, __VA_ARGS__)

class Logging {
public:
  enum class PrintType { ERROR = 0, LOG = 1, WARN = 2, DBG = 3 };
  static void debug(
      bool newLine, const char *file, const char *func, ...) noexcept;
  static void log(
      bool newLine, const char *file, const char *func, ...) noexcept;
  static void warn(
      bool newLine, const char *file, const char *func, ...) noexcept;
  static void error(
      bool newLine, const char *file, const char *func, ...) noexcept;
  static std::string format(bool newLine, PrintType type, const char *log,
      const char *file, const char *func);
  static std::string getPrefix(PrintType type, const char *module);
};
