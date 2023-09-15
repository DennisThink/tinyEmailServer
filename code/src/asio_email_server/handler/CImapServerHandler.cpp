#include "CImapServerHandler.h"
namespace tiny_email
{
    void CImapServerHandler::OnConnected()
    {
    }

    void CImapServerHandler::OnSend()
    {

    }

    void CImapServerHandler::OnRecive(const std::string strValue)
    {
        //m_proto.OnRecv(strValue);
        //m_client->Send(m_proto.GetSend());
    }
}