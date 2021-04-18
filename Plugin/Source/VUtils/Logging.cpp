#include <VUtils/Logging.h>
#include <cstdarg>
#include <cstdio>

#ifndef ERROR_COLOR
#define ERROR_COLOR 31
#endif
#ifndef INFO_COLOR
#define INFO_COLOR 34
#endif
#ifndef DEBUG_COLOR
#define DEBUG_COLOR 32
#endif
#ifndef WARN_COLOR
#define WARN_COLOR 33
#endif

void Logging::debug (bool newLine, const char* file, const char* func, ...) noexcept
{
    va_list args;
    va_start (args, func);
    auto log = va_arg (args, const char*);
    vprintf (Logging::format (newLine, PrintType::DBG, log, file, func).c_str(),
             args);
    va_end (args);
}

void Logging::log (bool newLine, const char* file, const char* func, ...) noexcept
{
    va_list args;
    va_start (args, func);
    auto log = va_arg (args, const char*);
    vprintf (Logging::format (newLine, PrintType::LOG, log, file, func).c_str(),
             args);
    va_end (args);
}

void Logging::error (bool newLine, const char* file, const char* func, ...) noexcept
{
    va_list args;
    va_start (args, func);
    auto log = va_arg (args, const char*);
    vprintf (Logging::format (newLine, PrintType::ERROR, log, file, func).c_str(),
             args);
    va_end (args);
}

void Logging::warn (bool newLine, const char* file, const char* func, ...) noexcept
{
    va_list args;
    va_start (args, func);
    auto log = va_arg (args, const char*);
    vprintf (Logging::format (newLine, PrintType::WARN, log, file, func).c_str(),
             args);
    va_end (args);
}

std::string Logging::format (bool newLine, PrintType type, const char* log, const char* file, const char* func)
{
    auto pre = getPrefix (
        type, std::string (VUtils::FileHandler::getFileName (file) + "::" + func).c_str());
    pre += log;
    pre += "\033[0m";
    if (newLine)
        pre += "\n";
    return pre;
}
std::string Logging::getPrefix (PrintType type, const char* s)
{
    switch (type)
    {
        case PrintType::ERROR:
            return "\033[1;" + std::to_string (ERROR_COLOR) + "m[ERROR][" + std::string (s) + "] >> ";
        case PrintType::DBG:
            return "\033[1;" + std::to_string (DEBUG_COLOR) + "m[DEBUG][" + std::string (s) + "] >> ";
        case PrintType::WARN:
            return "\033[1;" + std::to_string (WARN_COLOR) + "m[WARN][" + std::string (s) + "] >> ";
        default:
            return "\033[1;" + std::to_string (INFO_COLOR) + "m[LOG][" + std::string (s) + "] >> ";
    }
}
