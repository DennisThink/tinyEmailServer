#ifndef _CPP_EMAIL_IMAP_SERVER_HANDLER_H_
#define _CPP_EMAIL_IMAP_SERVER_HANDLER_H_
#include "networkHandler.h"
#include "CTcpClient.h"
#include "thirdLib.h"
#include "DataBaseInterface.h"
#include "../imap/imap_server_handler.h"
namespace tiny_email
{
    class CImapServerHandler:public INetWorkHandler
    {
    public:
        explicit CImapServerHandler(CDataBaseInterface_SHARED_PTR ptr, const std::string strDomainName);
       virtual ~CImapServerHandler()=default;
       virtual void OnConnected() override;
       virtual void OnSend() override;
       virtual void OnRecive(const std::string strValue) override;
       virtual void OnTimer() override{};
       virtual void OnClose() override{};
       virtual bool IsConnected() override{return m_client->isConnected();};
       virtual bool IsFinished() override { return m_proto->IsFinished(); }
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
       CTcpClient_ptr_t m_client;
       CDataBaseInterface_SHARED_PTR m_db;
       std::shared_ptr<CImapServerProtoHandler> m_proto;
    };
    using CImapServerHandler_SHARED_PTR = std::shared_ptr<CImapServerHandler>;
}
#endif