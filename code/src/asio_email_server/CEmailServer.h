#ifndef _CPP_EMAIL_EMAIL_SERVER_H_
#define _CPP_EMAIL_EMAIL_SERVER_H_
#include <string>
#include "thirdLib.h"
#include "CTcpServer.h"
#include "CMemDataBase.h"
#include "OldEmailServer.h"
#include "NewEmailServer.h"
#include "user_info.h"
namespace tiny_email 
{
    class CEmailServer 
    {
    public:
        explicit CEmailServer()
        {
        }

        void Start(const email_server_config& serverCfg);
    private:
        asio::io_context m_ioService;
        CSmtpServer_SHARED_PTR m_smtpHandler;
        CPop3Server_SHARED_PTR m_pop3Handler;
        CImapServer_SHARED_PTR m_imapHandler;

        CSmtpServerNew_SHARED_PTR m_smtpHandlerNew;
        CPop3ServerNew_SHARED_PTR m_pop3HandlerNew;
        CImapServerNew_SHARED_PTR m_imapHandlerNew;

        CDataBaseInterface_SHARED_PTR m_dataPtr;
    };
}
#endif