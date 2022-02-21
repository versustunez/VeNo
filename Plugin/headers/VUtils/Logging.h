#pragma once

#include "Logger.h"

#include <VUtils/FileHandler.h>
#include <string>

#ifdef DEBUG
#define DEBUGLOG(message, mod) VUtils::Logging::debugMod(message, mod);
#define DBGN(...)                                                              \
  VUtils::Logger::create(true, VUtils::PrintType::DBG, __FILE__, __FUNCTION__, \
                         __VA_ARGS__)
#else
#define DBGN(...)
#endif
#define ERR(...)                                                               \
  VUtils::Logger::create(true, VUtils::PrintType::ERROR, __FILE__,             \
                         __FUNCTION__, __VA_ARGS__)
#define LOG(...)                                                               \
  VUtils::Logger::create(true, VUtils::PrintType::LOG, __FILE__, __FUNCTION__, \
                         __VA_ARGS__)
#define WARN(...)                                                              \
  VUtils::Logger::create(true, VUtils::PrintType::WARN, __FILE__,              \
                         __FUNCTION__, __VA_ARGS__)
