#ifndef _CPP_EMAIL_SMTP_SERVER_HANDLER_H_
#define _CPP_EMAIL_SMTP_SERVER_HANDLER_H_
#include "networkHandler.h"
#include "CTcpClient.h"
#include "thirdLib.h"
#include "DataBaseInterface.h"
#include "../smtp/smtp_server_handler.h"
namespace tiny_email
{
    class CSmtpHandler:public INetWorkHandler
    {
    public:
       explicit CSmtpHandler(CDataBaseInterface_SHARED_PTR ptr,const std::string strDomainName);
       virtual ~CSmtpHandler()=default;
       virtual void OnConnected() override;
       virtual void OnSend() override;
       virtual void OnRecive(const std::string strValue) override;
       virtual void OnTimer() override{};
       virtual void OnClose() override{};
       virtual bool IsConnected() override{return m_client->isConnected();}
       virtual bool IsFinished() override { return m_proto->IsFinished();}
       void SetTcpSocket(CTcpClient_ptr_t tcpSock)
       {
           m_client = tcpSock;
           if(m_client->isConnected())
           {
                m_client->Send(m_proto->GetResponse());
           }
       }
       void Start()
       {
           m_client->Start();
       }
    private:
       std::shared_ptr<CSmtpServerProtoHandler> m_proto;
       CTcpClient_ptr_t m_client;
    };
    using CSmtpHandler_PTR = std::shared_ptr<CSmtpHandler>;
}
#endif