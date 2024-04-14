#include "CSmtpServerHandler.h"
namespace tiny_email
{
    CSmtpHandler::CSmtpHandler(log_ptr_t log, CDataBaseInterface_SHARED_PTR ptr)
    {
        this->m_log = log;
        m_proto = std::make_shared<CSmtpServerHandler>(ptr, "email.test.com");
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