#include "LogUtil.h"
int main(int argc, char *argv[])
{
    {
        auto logger = GetLogger();
        LOG_DEBUG(logger, "INFO");
        LOG_INFO(logger, "INFO");
        LOG_WARN(logger, "INFO");
        LOG_ERROR(logger, "INFO");
        LOG_FATAL(logger, "INFO");
    }
    {
        auto log2 = GetLogger();
        LOG_DEBUG(log2, "INFO {}",1);
        LOG_INFO(log2, "INFO {} {} ",2,"failed");
        LOG_WARN(log2, "INFO {} {} {}",1,1.2,3);
        LOG_ERROR(log2, "INFO");
        LOG_FATAL(log2, "INFO");
    }
    return 0;
}