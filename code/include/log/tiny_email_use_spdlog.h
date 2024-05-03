#ifndef _TINY_EMAIL_USE_SPD_LOG_H_
#define _TINY_EMAIL_USE_SPD_LOG_H_
#include <string>
#include <format>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "LogUtil.h"
namespace tiny_email
{
    template<typename... T>
    inline void Debug(const std::string strLog, T&&... args)
    {
        auto strMsg = std::vformat(strLog, std::make_format_args(args...));
        auto logger = GetLogger();
        if (logger)
        {
            logger->debug(strMsg);
        }
    }

    template<typename... T>
    inline void Info(const std::string strLog, T&&... args)
    {
        auto strMsg = std::vformat(strLog, std::make_format_args(args...));
        auto logger = GetLogger();
        if (logger)
        {
            logger->info(strMsg);
        }
    }

    template<typename... T>
    inline void Warn(const std::string strLog, T&&... args)
    {
        auto strMsg = std::vformat(strLog, std::make_format_args(args...));
        auto logger = GetLogger();
        if (logger)
        {
            logger->warn(strMsg);
        }
    }

    template<typename... T>
    inline void Error(const std::string strLog, T&&... args)
    {
        auto strMsg = std::vformat(strLog, std::make_format_args(args...));
        auto logger = GetLogger();
        if (logger)
        {
            logger->error(strMsg);
        }
    }
    template<typename... T>
    inline void Fatal(const std::string strLog, T&&... args)
    {
        auto strMsg = std::vformat(strLog, std::make_format_args(args...));
        auto logger = GetLogger();
        if (logger)
        {
            logger->error(strMsg);
        }
    }
}
#endif