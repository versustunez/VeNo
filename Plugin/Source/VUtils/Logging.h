#pragma once

#include "Logger.h"

#include <VUtils/FileHandler.h>
#include <string>

#ifdef DEBUG
#define VDBGN(log, ...)                                                        \
  VUtils::Logger::create(VUtils::Logger::format(log, __VA_ARGS__), __FILE__,   \
                         __FUNCTION__, VUtils::PrintType::DBG)
#define DBGN(...) VDBGN(__VA_ARGS__, "")
#else
#define DBGN(...)
#endif
#define VERR(log, ...)                                                         \
  VUtils::Logger::create(VUtils::Logger::format(log, __VA_ARGS__), __FILE__,   \
                         __FUNCTION__, VUtils::PrintType::ERROR)

#define VLOG(log, ...)                                                         \
  VUtils::Logger::create(VUtils::Logger::format(log, __VA_ARGS__), __FILE__,   \
                         __FUNCTION__, VUtils::PrintType::LOG)

#define VWARN(log, ...)                                                        \
  VUtils::Logger::create(VUtils::Logger::format(log, __VA_ARGS__), __FILE__,   \
                         __FUNCTION__, VUtils::PrintType::WARN)

#define ERR(...) VERR(__VA_ARGS__, "")
#define LOG(...) VLOG(__VA_ARGS__, "")
#define WARN(...) VWARN(__VA_ARGS__, "")