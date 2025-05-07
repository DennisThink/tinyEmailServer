#include "smtp_server.h"
#include "TCPServer.h"
#include "Socket.h"
#include "smtp_server_handler.h"
#include "SqliteDataBase.h"
#include "LogUtil.h"
#include "ProtoUtil.h"
auto logFunc = [](const std::string strRsp) {};
static auto g_log = GetLogger();
void StartSmtpServer(const tiny_email::email_server_config &serverCfg)
{

    CTCPServer server(logFunc, std::to_string(serverCfg.m_smtpServer.port_));
    ASocket::Socket client;
    char buff[128] = {0};
    int recvLen = 0;
    auto dbPtr = std::make_shared<tiny_email::CSqliteDataBase>(serverCfg.m_strDataBaseName);
            //Test Data begin
        {
            if(!dbPtr->IsUserExist("test1@test.com"))
            {
                dbPtr->AddUser("test1@test.com","test1_pass");
            }
            if(!dbPtr->IsUserExist("test2@test.com"))
            {
                dbPtr->AddUser("test2@test.com","test2_pass");
            }
        }
        //Test Data end
        if (server.Listen(client,-1))
        {
            std::string strDomain = tiny_email::CProtoUtil::GetSmtpDomainFromMainDomain(serverCfg.m_strDomain);
            tiny_email::CSmtpServerProtoHandler handler(dbPtr, strDomain);

            while (true)
            {
                std::string strRsp = handler.GetResponse();
                if (!strRsp.empty())
                {
                    if (server.Send(client, strRsp))
                    {
                        LOG_INFO(g_log, "S:{}", strRsp);
                    }
                    else
                    {
                        LOG_ERROR(g_log, "Send Failed {}", strRsp);
                        break;
                    }
                }
                memset(buff, 128, 0);
                recvLen = -1;
                recvLen = server.Receive(client, buff, 127, false);
                if (recvLen <= 0)
                {
                    break;
                }
                if (recvLen > 0)
                {
                    std::string strReq(buff, recvLen);
                    LOG_INFO(g_log, "C:{}", strReq);
                    if (handler.OnClientReq(strReq))
                    {
                    }
                    else
                    {
                        LOG_ERROR(g_log, "Handle Client Failed");
                    }
                }
                if (handler.IsFinished())
                {
                    LOG_INFO(g_log, "Break Finished");
                    break;
                }
            }
            server.Disconnect(client);
        }
}