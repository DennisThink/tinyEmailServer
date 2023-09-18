#include "LogBase.h"
#include <iostream>
namespace tiny_email
{
    template<typename ...T>
    inline void LogBase::Debug(const std::string strLog,T... args)
    {
        Log(strLog,args...);
    }
    
    template<typename ...T>
    inline void LogBase::Info(const std::string strLog,T... args)
    {
        Log(strLog,args...);
    }

    template<typename ...T>
    inline void LogBase::Warn(const std::string strLog,T... args)
    {
        Log(strLog,args...);
    }

    template<typename ...T>
    inline void LogBase::Error(const std::string strLog,T... args)
    {
     Log(strLog,args...);
    }
    template<typename ...T>
    inline void LogBase::Fatal(const std::string strLog,T... args)
    {
      Log(args...);
    }

    template<typename U,typename...T>
    inline void LogBase::Log(U arg,T... args)
    {
        std::cout<<arg;
        Log(args...);
    }

    template<typename U>
    inline void LogBase::Log(U arg)
    {
        std::cout<<arg<<std::endl;
    }
}