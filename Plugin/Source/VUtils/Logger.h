#pragma once

#include <VUtils/LogInclude.h>
#include <string>

namespace VUtils {
enum class PrintType { ERROR = 0, LOG = 1, WARN = 2, DBG = 3 };

class Logger {
public:
  static void create(const std::string &output,
                     PrintType type = PrintType::LOG);
  static void create(const std::string &output, const char *file,
                     const char *func, PrintType type = PrintType::LOG);
  template <typename S, typename... Args>
  static std::string format(const S &format, Args &&...args) {
    return fmt::format(format, args...);
  }

protected:
  static int getColor(PrintType type);
  static std::string getType(PrintType type);
  static std::string getPlace(const char *file, const char *name);
};
} // namespace VUtils