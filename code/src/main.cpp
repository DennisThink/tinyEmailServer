#include <iostream>
#include "smtp_server.h"
int main(int argc,char * argv[])
{
    std::cout<<"tinyEmailServer"<<std::endl;
    StartSmtpServer();
    return 0;
}