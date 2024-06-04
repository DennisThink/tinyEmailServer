#include "CEmailServer.h"
#include "CMemDataBase.h"
#include "SqliteDataBase.h"
#include "CNetServerHandler.h"
namespace tiny_email
{
    void CEmailServer::Start(const email_server_config& serverCfg)
    {
        m_dataPtr = std::make_shared<CSqliteDataBase>(serverCfg.m_strDataBaseName);
        {
            //Test Data begin
            {
                if (!m_dataPtr->IsUserExist("test1@test.com"))
                {
                    m_dataPtr->AddUser("test1@test.com", "test1_pass");
                }
                if (!m_dataPtr->IsUserExist("test2@test.com"))
                {
                    m_dataPtr->AddUser("test2@test.com", "test2_pass");
                }
            }
        }
        log_ptr_t g_console = spdlog::stdout_color_mt("email_server");
        {
            m_smtpHandler = std::make_shared<CSmtpServer>(m_ioService, serverCfg.m_smtpServer.strIp_, serverCfg.m_smtpServer.port_, m_dataPtr, serverCfg.m_strDomain);
            m_smtpHandler->Start();

            m_pop3Handler = std::make_shared<CPop3Server>(m_ioService, serverCfg.m_pop3Server.strIp_, serverCfg.m_pop3Server.port_, m_dataPtr, serverCfg.m_strDomain);
            m_pop3Handler->Start();

            m_imapHandler = std::make_shared<CImapServer>(m_ioService, serverCfg.m_imapServer.strIp_, serverCfg.m_imapServer.port_, m_dataPtr, serverCfg.m_strDomain);
            m_imapHandler->Start();
        }
        m_ioService.run();
    }
}