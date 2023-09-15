#include "CSmtpServerHandler.h"
namespace tiny_email
{
    void CSmtpHandler::OnConnected()
    {
    }

    void CSmtpHandler::OnSend()
    {

    }

    void CSmtpHandler::OnRecive(const std::string strValue)
    {
        //m_proto.OnRecv(strValue);
        //m_client->Send(m_proto.GetSend());
        //if(m_proto.IsFinish())
        //{
        //    m_client->Close();
        //}
    }
}