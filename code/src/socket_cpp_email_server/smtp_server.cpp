#include "smtp_server.h"
#include "TCPServer.h"
#include "Socket.h"
#include "smtp_server_handler.h"
auto logFunc = [](const std::string strRsp) {};
void StartSmtpServer()
{
    std::string strPort = "25";
    CTCPServer server(logFunc, strPort);
    ASocket::Socket client;
    char buff[128] = {0};
    int recvLen = 0;
    while (true)
    {
        if (server.Listen(client, 1000))
        {
            tiny_email::CSmtpServerHandler handler;

            while (true)
            {

                std::string strRsp = handler.GetResponse();
                if (!strRsp.empty())
                {
                    if (server.Send(client, strRsp))
                    {
                        std::cout << "S: " << strRsp << std::endl;
                    }
                    else
                    {
                        std::cout << "Send Failed" << std::endl;
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
                    std::cout << "Recv: " << strReq << std::endl;
                    if (handler.OnClientReq(strReq))
                    {
                    }
                    else
                    {
                        std::cout<<"Handle Client Failed"<<std::endl;
                    }
                }
            }
            server.Disconnect(client);
        }
    }
}