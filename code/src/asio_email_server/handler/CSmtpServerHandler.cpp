#include "CSmtpServerHandler.h"
namespace tiny_email
{
    CSmtpHandler::CSmtpHandler(CDataBaseInterface_SHARED_PTR ptr, const std::string strDomainName)
    {
        m_proto = std::make_shared<CSmtpServerProtoHandler>(ptr, strDomainName);
    }
    void CSmtpHandler::OnConnected()
    {
    }

    void CSmtpHandler::OnSend()
    {

    }

    void CSmtpHandler::OnRecive(const std::string strValue)
    {
        if (!strValue.empty())
        {
            m_proto->OnClientReq(strValue);
            m_client->Send(m_proto->GetResponse());
            if (m_proto->IsFinished())
            {
                m_client->Close();
            }
        }
    }
}