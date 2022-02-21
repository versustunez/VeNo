#include <VUtils/FileHandler.h>
#include <VUtils/LogDefines.h>
#include <VUtils/Logger.h>
#include <ctime>
#include <iomanip>
#include <iostream>

namespace VUtils {
bool Logger::showDate = LOG_SHOW_DATE;
void Logger::log(const std::string &log) { Logger::get().add(log); }
std::string Logger::format(bool newLine, PrintType type, const char *log,
                           const char *file, const char *func) {
  auto pre = getPrefix(
      type, std::string(VUtils::FileHandler::getFileName(file) + "::" + func)
                .c_str());
  pre += log;
  pre += "\033[0m";
  if (newLine)
    pre += "\n";
  return pre;
}
std::string Logger::getPrefix(PrintType type, const char *module) {
  std::stringstream stream;
  stream << "\033[1;";
  std::string logType = INFO_KEY;
  switch (type) {
  case PrintType::ERROR: {
    stream << ERROR_COLOR;
    logType = ERROR_KEY;
    break;
  }
  case PrintType::DBG: {
    stream << DEBUG_COLOR;
    logType = DEBUG_KEY;
    break;
  }
  case PrintType::WARN: {
    stream << WARN_COLOR;
    logType = WARN_KEY;
    break;
  }
  case PrintType::LOG: stream << INFO_COLOR; break;
  }
  stream << "m";
  stream << "[" << logType << "]";
  if (showDate) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    stream << "[" << std::put_time(&tm, TIME_FORMAT) << "]";
  }
  stream << "[" << module << "] >> ";
  return stream.str();
}
void Logger::add(const std::string &log) { m_queue.push(log); }
Logger::Logger() : m_thread(std::thread(&Logger::run, this)) {}
Logger::~Logger() {
  if (!m_queue.isClosed())
    Logger::close();
}
void Logger::run() {
  while (auto queueItem = m_queue.waitAndPop()) {
    if (!queueItem.has_value())
      return;
    std::cout << queueItem.value();
    if (m_queue.size() > 0) {
      int index = 0;
      while (auto item = m_queue.pop()) {
        if (!item.has_value() || index > 10)
          break;
        index++;
        std::cout << item.value();
      }
    }
  }
}
void Logger::create(bool newLine, PrintType type, const char *file,
                    const char *func, ...) {
  va_list args;
  va_start(args, func);
  auto log = va_arg(args, const char *);
  auto rawLog = Logger::format(newLine, type, log, file, func);
  auto toLog = rawLog.c_str();
  int length = findLength(toLog, args);
  char *buffer = new char[length];
  buffer[length - 1] = '\0';
  vsnprintf(buffer, length, toLog, args);
  Logger::log(buffer);
  delete[] buffer;
  va_end(args);
}
int Logger::findLength(const char *log, va_list args) {
  va_list newArg;
  va_copy(newArg, args);
  return vsnprintf(nullptr, 0, log, newArg) + 1;
}
void Logger::close() {
  m_queue.flush();
  m_thread.join();
}
} // namespace VUtils