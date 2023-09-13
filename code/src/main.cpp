#include <iostream>
#include "smtp_server.h"
#include "pop3_server.h"
int main(int argc,char * argv[])
{
    std::cout<<"tinyEmailServer"<<std::endl;
    //StartSmtpServer();
    StartPop3Server();
    return 0;
}