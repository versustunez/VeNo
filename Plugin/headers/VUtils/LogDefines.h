#pragma once
#define ERROR_KEY "ERROR"
#define INFO_KEY "INFO"
#define DEBUG_KEY "DEBUG"
#define WARN_KEY "WARN"

#ifndef LOG_SHOW_DATE
#define LOG_SHOW_DATE true
#endif

#ifndef ERROR_COLOR
#define ERROR_COLOR "31"
#endif
#ifndef INFO_COLOR
#define INFO_COLOR "34"
#endif
#ifndef DEBUG_COLOR
#define DEBUG_COLOR "32"
#endif
#ifndef WARN_COLOR
#define WARN_COLOR "33"
#endif
#ifndef TIME_FORMAT
#define TIME_FORMAT "%d-%m-%Y %H:%M:%S"
#endif
