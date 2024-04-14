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
            m_proto->OnClientReq(strValue);
            m_client->Send(m_proto->GetResponse());
        }
    }
}