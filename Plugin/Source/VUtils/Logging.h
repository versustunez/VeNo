#pragma once

#include "Logger.h"

#include <VUtils/FileHandler.h>
#include <string>

#if 0
#ifdef DEBUG
#define DBGN(log, ...)                                                         \
  VUtils::Logger::create(fmt::format(log __VA_OPT__(, ) __VA_ARGS__),          \
                         __FILE__, __FUNCTION__, VUtils::PrintType::Debug)
#else
#define DBGN(...)
#endif
#define ERR(log, ...)                                                          \
  VUtils::Logger::create(fmt::format(log __VA_OPT__(, ) __VA_ARGS__),          \
                         __FILE__, __FUNCTION__, VUtils::PrintType::Error)

#define LOG(log, ...)                                                          \
  VUtils::Logger::create(fmt::format(log __VA_OPT__(, ) __VA_ARGS__),          \
                         __FILE__, __FUNCTION__, VUtils::PrintType::Log)

#define WARN(log, ...)                                                         \
  VUtils::Logger::create(fmt::format(log __VA_OPT__(, ) __VA_ARGS__),          \
                         __FILE__, __FUNCTION__, VUtils::PrintType::Warn)
#else
#define DBGN(...)
#define ERR(...)
#define LOG(...)
#define WARN(...)
#endif
