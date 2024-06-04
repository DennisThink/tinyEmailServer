#ifndef _CPP_EMAIL_NET_SERVER_HANDLER_H_
#define _CPP_EMAIL_NET_SERVER_HANDLER_H_
#include "networkHandler.h"
#include "CTcpClient.h"
#include "thirdLib.h"
#include "DataBaseInterface.h"
#include "../common/CEmailServerProtoHandler.h"
namespace tiny_email
{
    template<typename T>
    class CNetServerHandler:public INetWorkHandler
    {
    public:
        explicit CNetServerHandler(CDataBaseInterface_SHARED_PTR ptr, const std::string strDomainName)
        {
            m_proto = std::make_shared<T>(ptr, strDomainName);
        }
        ~CNetServerHandler()=default;
        void OnConnected() override
        {

        }
        void OnSend() override
        {

        }
        void OnRecive(const std::string strValue) override
       {
            if (m_proto)
            {
                m_proto->OnClientReq(strValue);
                std::string strRsp = m_proto->GetResponse();
                m_client->Send(strRsp);
            }
       }
        void OnTimer() override{};
        void OnClose() override{};
        bool isConnected() override{return m_client->isConnected();};
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
       CEmailServerProtoInterface_PTR m_proto;
    };
}
#endif