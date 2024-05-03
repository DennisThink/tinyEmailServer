#ifndef TINY_EMAIL_DEFAULT_LOG_H_
#define TINY_EMAIL_DEFAULT_LOG_H_
#include <string>
#include <format>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
namespace tiny_email 
{
    template<typename... T>
    inline void Log(const std::string strfmt, T&&... args)
    {
        auto strLog = std::vformat(strfmt, std::make_format_args(args...));
        std::cout << strLog << std::endl;
    }
    template<typename... T>
    inline void Debug(const std::string strLog, T&&... args)
    {
        Log(strLog, std::forward<T>(args)...);
    }

    template<typename... T>
    inline void Info(const std::string strLog, T&&... args)
    {
        Log(strLog, std::forward<T>(args)...);
    }

    template<typename... T>
    inline void Warn(const std::string strLog, T&&... args)
    {
        Log(strLog, std::forward<T>(args)...);
    }

    template<typename... T>
    inline void Error(const std::string strLog, T&&... args)
    {
        Log(strLog, std::forward<T>(args)...);
    }
    template<typename... T>
    inline void Fatal(const std::string strLog, T&&... args)
    {
        Log(strLog, std::forward<T>(args)...);
    }
}
#endif