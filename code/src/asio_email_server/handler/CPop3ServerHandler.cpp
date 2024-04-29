#include "CPop3ServerHandler.h"
namespace tiny_email
{
    CPop3Handler::CPop3Handler(CDataBaseInterface_SHARED_PTR ptr,const std::string strDomainName)
    {
        m_proto = std::make_shared<CPop3ServerProtoHandler>(ptr, strDomainName);
    }

    void CPop3Handler::OnSend()
    {

    }

    void CPop3Handler::OnRecive(const std::string strValue)
    {
      
        if (!strValue.empty())
        {
            tiny_email::Info("Client: {} ", strValue);
            m_proto->OnClientReq(strValue);
            std::string strRsp = m_proto->GetResponse();
            tiny_email::Info("Server: {} ", strRsp);
            m_client->Send(strRsp);
            if (m_proto->IsFinished())
            {
                m_client->Close();
            }
        }
    }
}