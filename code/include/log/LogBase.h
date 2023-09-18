#ifndef _TINY_EMAIL_LOG_BASE_H_
#define _TINY_EMAIL_LOG_BASE_H_
#include <string>
#include <memory>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
namespace tiny_email
{
    class LogBase
    {
    public:
        template<typename...T>
        void Debug(const std::string strLog,T... args);
        template<typename...T>
        void Info(const std::string strLog,T... args);
        template<typename...T>
        void Warn(const std::string strLog,T... args);
        template<typename...T>
        void Error(const std::string strLog,T... args);
        template<typename...T>
        void Fatal(const std::string strLog,T... args);
    protected:
        template<typename U,typename...T>
        void Log(U arg,T... args);

        template<typename U>
        void Log(U arg);
    };
}
#endif