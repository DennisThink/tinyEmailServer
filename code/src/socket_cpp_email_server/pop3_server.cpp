#include "pop3_server.h"
#include "pop3_server_handler.h"
#include "TCPServer.h"
#include "Socket.h"
#include "SqliteDataBase.h"
#include "LogUtil.h"
auto logFunc2 = [](const std::string strRsp) {};
static auto g_log = GetLogger();
void StartPop3Server()
{
    std::string strPort = "2110";
    CTCPServer server(logFunc2, strPort);
    ASocket::Socket client;
    char buff[128] = {0};
    int recvLen = 0;
    //auto logger = std::make_shared<tiny_email::LogSpdlog>();
    while (true)
    {
        if (server.Listen(client, 1000))
        {
            auto dbPtr = std::make_shared<tiny_email::CSqliteDataBase>("sock_cpp_email.db");
            tiny_email::CPop3ServerHandler handler(dbPtr);

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