#pragma once

#include <format>
#include <string>

namespace VUtils {
enum class PrintType { Error = 0, Log = 1, Warn = 2, Debug = 3 };

class Logger {
public:
  static void create(const std::string &output,
                     PrintType type = PrintType::Log);
  static void create(const std::string &output, const char *file,
                     const char *func, PrintType type = PrintType::Log);

protected:
  static int getColor(PrintType type);
  static std::string getType(PrintType type);
  static std::string getPlace(const char *file, const char *name);
};
} // namespace VUtils