#include "CPop3ServerHandler.h"
namespace tiny_email
{

    void CPop3Handler::OnSend()
    {

    }

    void CPop3Handler::OnRecive(const std::string strValue)
    {
        if (!strValue.empty())
        {
            m_proto->OnClientReq(strValue);
            m_client->Send(m_proto->GetResponse());
        }
    }
}