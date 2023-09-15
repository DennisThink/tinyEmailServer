#include "CEmailServer.h"
#include "CMemDataBase.h"
namespace tiny_email
{
    void CEmailServer::Start()
    {
        m_dataPtr = std::make_shared<CMemDataBase>();
        log_ptr_t g_console = spdlog::stdout_color_mt("email_server");
        m_smtpHandler = std::make_shared<CSmtpServer>(m_ioService,g_console,"127.0.0.1",25,m_dataPtr);
        m_smtpHandler->Start();

        m_pop3Handler = std::make_shared<CPop3Server>(m_ioService,g_console,"127.0.0.1",110,m_dataPtr);
        m_pop3Handler->Start();

        m_imapHandler = std::make_shared<CImapServer>(m_ioService,g_console,"127.0.0.1",143,m_dataPtr);
        m_imapHandler->Start();
        m_ioService.run();
    }
}