#include "CImapServerHandler.h"
namespace tiny_email
{
    void CImapServerHandler::OnConnected()
    {
    }

    void CImapServerHandler::OnSend()
    {

    }
    CImapServerHandler::CImapServerHandler(CDataBaseInterface_SHARED_PTR ptr, const std::string strDomainName)
    {
        m_db = ptr;
        m_proto = std::make_shared<CImapServerProtoHandler>(ptr, strDomainName);
    }
    void CImapServerHandler::OnRecive(const std::string strValue)
    {
        if (m_proto)
        {
            m_proto->OnClientReq(strValue);
            std::string strRsp = m_proto->GetResponse();
            m_client->Send(strRsp);
        }
    }
}