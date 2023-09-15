#ifndef _CPP_EMAIL_THIRD_LIB_H_
#define _CPP_EMAIL_THIRD_LIB_H_
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
using log_ptr_t = std::shared_ptr<spdlog::logger>;

#define ASIO_STANDALONE
#include "asio/asio.hpp"

#endif