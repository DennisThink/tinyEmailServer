#include "pop3_server.h"
#include "pop3_server_handler.h"
#include "TCPServer.h"
#include "Socket.h"
#include "SqliteDataBase.h"
#include "LogUtil.h"
#include "ProtoUtil.h"
auto logFunc2 = [](const std::string strRsp) {};
static auto g_log = GetLogger();
void StartPop3Server(const tiny_email::email_server_config& serverCfg)
{
    CTCPServer server(logFunc2, std::to_string(serverCfg.m_pop3Server.port_));
    ASocket::Socket client;
    char buff[128] = {0};
    int recvLen = 0;
    while (true)
    {
        if (server.Listen(client, 1000))
        {
            auto dbPtr = std::make_shared<tiny_email::CSqliteDataBase>(serverCfg.m_strDataBaseName);
            tiny_email::CPop3ServerHandler handler(dbPtr,tiny_email::CProtoUtil::GetPop3DomainFromMainDomain(serverCfg.m_strDomain));

            while (true)
            {

                std::string strRsp = handler.GetResponse();
                if (!strRsp.empty())
                {
                    if (server.Send(client, strRsp))
                    {
                        LOG_INFO(g_log,"S: {}",strRsp);
                    }
                    else
                    {
                        LOG_ERROR(g_log,"Send Failed:  {}",strRsp);
                        break;
                    }
                }
                memset(buff, 128, 0);
                recvLen = -1;
                recvLen = server.Receive(client, buff, 128, false);
                if (recvLen <= 0)
                {
                    break;
                }
                if (recvLen > 0)
                {
                    std::string strReq(buff,recvLen);
                    LOG_INFO(g_log,"C: {}",strReq);
                    if (handler.OnClientReq(strReq))
                    {
                    }
                    else
                    {
                        LOG_ERROR(g_log,"Handle Client Req Failed");
                    }
                }
            }
            server.Disconnect(client);
        }
    }
}