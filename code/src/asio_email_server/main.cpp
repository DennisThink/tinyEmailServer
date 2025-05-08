#include "CEmailServer.h"
#include <iostream>
#include <string>
int main(int argc,char* argv[])
{
    std::cout<<argc<<argv[0]<<std::endl;
    std::string strConfigName="default.cfg";
    std::string strVersion = "0.1.0";
    {
        tiny_email::CEmailServer server;
        tiny_email::email_server_config servercfg;
        servercfg.m_strDataBaseName = "asio_server.db";
        servercfg.m_strDomain = "email.test.com";
        servercfg.m_smtpServer.strIp_ = "127.0.0.1";
        servercfg.m_smtpServer.port_ = 2125;
        servercfg.m_pop3Server.strIp_ = "127.0.0.1";
        servercfg.m_pop3Server.port_ = 2110;
        servercfg.m_imapServer.strIp_ = "127.0.0.1";
        servercfg.m_imapServer.port_ = 2143;

        std::cout << servercfg.toString() << std::endl;
        server.Start(servercfg);
    }
    {
        {
            std::string strFileName;
            tiny_email::email_server_config serverCfg;
            tiny_email::CEmailServer server;
            if (ParseConfigFromFile(strFileName, serverCfg))
            {
                std::cout << serverCfg.toString() << std::endl;
                server.Start(serverCfg);
            }
            
        }

    }
    return 0;
}