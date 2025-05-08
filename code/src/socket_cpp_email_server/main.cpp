#include <iostream>
#include "smtp_server.h"
#include "pop3_server.h"
#include <string.h>
int main(int argc,char * argv[])
{
    tiny_email::email_server_config emailCfg;
    emailCfg.m_strDomain = "email.test.com";
    emailCfg.m_strDataBaseName = "socket_email_server.db";
    emailCfg.m_smtpServer.strIp_ = "127.0.0.1";
    emailCfg.m_smtpServer.port_=3025;
    emailCfg.m_pop3Server.strIp_ = "127.0.0.1";
    emailCfg.m_pop3Server.port_=3110;
    std::cout << emailCfg.toString() << std::endl;
    std::cout << "[./program.exe smtp] to start smtp server" << std::endl;
    std::cout << "[./program.exe pop3] to start pop3 server" << std::endl;
    if(argc > 1)
    {
        if(strcmp(argv[1],"smtp")==0)
        {
             StartSmtpServer(emailCfg);
        }
        else
        {
            StartPop3Server(emailCfg);
        }
    }
    std::cout<<"tinyEmailServer"<<std::endl;
   
 
    return 0;
}