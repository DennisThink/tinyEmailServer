#include "Log.h"
int main(int argc, char *argv[])
{
    std::cout << argc << "  " << argv[0] << std::endl;
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