#include "Log.h"
int main(int argc, char *argv[])
{
    std::cout << argc << "  " << argv[0] << std::endl;
    //{
    //    auto logger = GetLogger();
    //    LOG_DEBUG(logger, "INFO");
    //    LOG_INFO(logger, "INFO");
    //    LOG_WARN(logger, "INFO");
    //    LOG_ERROR(logger, "INFO");
    //    LOG_FATAL(logger, "INFO");
    //}
    //{
    //    auto log2 = GetLogger();
    //    LOG_DEBUG(log2, "INFO {}",1);
    //    LOG_INFO(log2, "INFO {} {} ",2,"failed");
    //    LOG_WARN(log2, "INFO {} {} {}",1,1.2,3);
    //    LOG_ERROR(log2, "INFO");
    //    LOG_FATAL(log2, "INFO");
    //}
    //{
    //    std::cout << std::format("Hello {}!\n", "world");

    //    std::string fmt;
    //    for (int i{}; i != 3; ++i)
    //    {
    //        fmt += "{} "; // constructs the formatting string
    //        std::cout << fmt << " : ";
    //        std::cout << dyna_print(fmt, "alpha", 'Z', 3.14, "unused");
    //        std::cout << '\n';
    //    }
    //}
    {
        std::string strFmt = "Hello";
        tiny_email::Debug("Debug Hello");
        tiny_email::Debug("Debug Hello {}", 1);
        tiny_email::Info("Info Hello");
        tiny_email::Info("Info hello {}", "1234");
        tiny_email::Warn("Warn Hello");
        tiny_email::Warn("Warn hello {}", 12.34);
    }
    return 0;
}