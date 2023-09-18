#include "smtp_server.h"
#include "TCPServer.h"
#include "Socket.h"
#include "smtp_server_handler.h"
#include "SqliteDataBase.h"
#include "LogUtil.h"
auto logFunc = [](const std::string strRsp) {};
static auto g_log = GetLogger();
void StartSmtpServer()
{
    std::string strPort = "2125";
    CTCPServer server(logFunc, strPort);
    ASocket::Socket client;
    char buff[128] = {0};
    int recvLen = 0;
    auto dbPtr = std::make_shared<tiny_email::CSqliteDataBase>("sock_cpp_email.db");
    while (true)
    {
        if (server.Listen(client, 1000))
        {
            
            tiny_email::CSmtpServerHandler handler(dbPtr);

            while (true)
            {
                std::string strRsp = handler.GetResponse();
                if (!strRsp.empty())
                {
                    if (server.Send(client, strRsp))
                    {
                        LOG_INFO(g_log,"S:{}",strRsp);
                    }
                    else
                    {
                        LOG_ERROR(g_log,"Send Failed {}",strRsp);
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
                    LOG_INFO(g_log,"C:{}",strReq);
                    if (handler.OnClientReq(strReq))
                    {
                    }
                    else
                    {
                        LOG_ERROR(g_log,"Handle Client Failed");
                    }
                }
                if(handler.IsFinished())
                {
                    LOG_INFO(g_log,"Break Finished");
                    break;
                }
            }
            server.Disconnect(client);
        }
    }
}