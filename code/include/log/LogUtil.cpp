#include "LogUtil.h"
static std::shared_ptr<spdlog::logger> g_log=nullptr;
std::shared_ptr<spdlog::logger> GetLogger()
{
    if(g_log)
    {
        return g_log;
    }
    else
    {
        g_log =spdlog::stdout_color_mt("console");
        return g_log;
    }
}