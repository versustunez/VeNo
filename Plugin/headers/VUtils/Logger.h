#pragma once

#include <VUtils/Storage/SafeQueue.h>
#include <cstdarg>
#include <string>
#include <thread>

namespace VUtils {
enum class PrintType { ERROR = 0, LOG = 1, WARN = 2, DBG = 3 };

class Logger {
public:
  static Logger &get() {
    static Logger _instance;
    return _instance;
  }
  void add(const std::string &log);
  void run();
  static std::string format(bool newLine, PrintType type, const char *log,
                            const char *file, const char *func);
  static std::string getPrefix(PrintType type, const char *module);
  static void log(const std::string &log);
  static void create(bool newLine, PrintType type, const char *file,
                     const char *func, ...);
  void close();
  static bool showDate;

private:
  Logger();
  ~Logger();
  VUtils::SafeQueue<std::string> m_queue{};
  std::thread m_thread;
  static int findLength(const char *log, va_list args);
};
} // namespace VUtils
