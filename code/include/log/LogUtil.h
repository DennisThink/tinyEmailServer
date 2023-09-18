#ifndef _TINY_EMAIL_LOG_UTIL_H_
#define _TINY_EMAIL_LOG_UTIL_H_
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#if 1
std::shared_ptr<spdlog::logger> GetLogger();
#define LOG_DEBUG(log,...){if(log){log->debug(__VA_ARGS__);}}
#define LOG_INFO(log,...){if(log){log->info(__VA_ARGS__);}}
#define LOG_WARN(log,...){if(log){log->warn(__VA_ARGS__);}}
#define LOG_ERROR(log,...){if(log){log->error(__VA_ARGS__);}}
#define LOG_FATAL(log,...){if(log){log->error(__VA_ARGS__);}}
#endif
#if 0
#define LOG_DEBUG(log,...){}
#define LOG_INFO(log,...){}
#define LOG_WARN(log,...){}
#define LOG_ERROR(log,...){}
#define LOG_FATAL(log,...){}
#endif

#endif