#include "CEmailServer.h"
#include <iostream>
int main(int argc,char* argv[])
{
    std::cout<<argc<<argv[0]<<std::endl;
    using namespace tiny_email;
    CEmailServer server;
    email_server_config servercfg;
    servercfg.m_strDataBaseName="asio_server.db";
    servercfg.m_strDomain = "email.test.com";
    servercfg.m_smtpServer.port_=2125;
    servercfg.m_pop3Server.port_=2110;
    server.Start(servercfg);
    return 0;
}