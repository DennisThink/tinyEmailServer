#include "smtp_server_handler.h"
#include "SqliteDataBase.h"
#include "CProtoCmd.h"
#include "ProtoUtil.h"
#include "LogUtil.h"
#include <iostream>
#include "CEmailServerProtoHandler.h"
namespace tiny_email
{
    static auto g_log = GetLogger();
    CSmtpServerProtoHandler::CSmtpServerProtoHandler(CDataBaseInterface_SHARED_PTR dbPtr,const std::string strDomainName):CEmailServerProtoHandlerInterface(dbPtr,strDomainName)
    {
        m_step = Smtp_Server_Step_t::SMTP_ON_CONNECT;
        m_strResponse = GetNextStepCmd(m_step);
        m_step = Smtp_Server_Step_t::SMTP_RECV_HELO_FIRST;
        m_bFinished = false;
    }

    bool CSmtpServerProtoHandler::IsFinished()
    {
        return m_bFinished;
    }

    bool (CSmtpServerProtoHandler::*pFunc)(const std::string strReq);
    struct SmtpServerStepElem
    {
        Smtp_Server_Step_t curStep;
        Smtp_Server_Step_t nextStep;
        decltype(pFunc)  callback_func;
    };
 
    bool CSmtpServerProtoHandler::OnClientReq(const std::string strValue)
    {
        //或许直接指定下一步的状态不是一个好的选择，此处考虑优化
        static struct SmtpServerStepElem stepArray[] =
        {
            {Smtp_Server_Step_t::SMTP_BEGIN, Smtp_Server_Step_t::SMTP_ON_CONNECT,&CSmtpServerProtoHandler::OnEhloReq},
            {Smtp_Server_Step_t::SMTP_ON_CONNECT, Smtp_Server_Step_t::SMTP_RECV_HELO_FIRST,&CSmtpServerProtoHandler::OnEhloReq},
            {Smtp_Server_Step_t::SMTP_RECV_HELO_FIRST, Smtp_Server_Step_t::SMTP_RECV_AUTH_LOGIN_REQ,&CSmtpServerProtoHandler::OnEhloReq},
            {Smtp_Server_Step_t::SMTP_RECV_AUTH_LOGIN_REQ, Smtp_Server_Step_t::SMTP_RECV_PASS_WORD_REQ,&CSmtpServerProtoHandler::OnAuthLoginReq},
            {Smtp_Server_Step_t::SMTP_RECV_PASS_WORD_REQ, Smtp_Server_Step_t::SMTP_NAME_PASS_VERIFY,&CSmtpServerProtoHandler::OnPasswordReq},
            {Smtp_Server_Step_t::SMTP_NAME_PASS_VERIFY, Smtp_Server_Step_t::SMTP_RECV_MAIL_FROM_REQ,&CSmtpServerProtoHandler::OnNamePassVerifyReq},
            {Smtp_Server_Step_t::SMTP_RECV_MAIL_FROM_REQ, Smtp_Server_Step_t::SMTP_RECV_RCPT_TO_REQ,&CSmtpServerProtoHandler::OnMailFromReq},
            {Smtp_Server_Step_t::SMTP_RECV_RCPT_TO_REQ, Smtp_Server_Step_t::SMTP_RECV_DATA_REQ,&CSmtpServerProtoHandler::OnRcptToReq},
            {Smtp_Server_Step_t::SMTP_RECV_DATA_REQ, Smtp_Server_Step_t::SMTP_RECV_EMAIL_DATA,&CSmtpServerProtoHandler::OnDataReq},
            {Smtp_Server_Step_t::SMTP_RECV_EMAIL_DATA, Smtp_Server_Step_t::SMTP_RECV_EMAIL_DATA,&CSmtpServerProtoHandler::OnData},
            {Smtp_Server_Step_t::SMTP_END,Smtp_Server_Step_t::SMTP_END,&CSmtpServerProtoHandler::OnClientReq},
        };

        if (!strValue.empty())
        {
            for (std::size_t i = 0; i < sizeof(stepArray) / sizeof(stepArray[0]); i++)
            {
                if (m_step == stepArray[i].curStep)
                {
                    bool ret = (this->*stepArray[i].callback_func)(strValue);
                    if(ret)
                    {
                        m_strResponse = GetNextStepCmd(m_step);
                        m_step = stepArray[i].nextStep;
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
        return false;
    }
    std::string CSmtpServerProtoHandler::GetNextStepCmd(const Smtp_Server_Step_t &step)
    {
        if (step == Smtp_Server_Step_t::SMTP_ON_CONNECT)
        {
            std::string strCode = "220 ";
            std::string strTail = " Anti-spam GT for Coremail System\r\n";
            return strCode + m_strEmailDomain + strTail;
        }
        if (step == Smtp_Server_Step_t::SMTP_RECV_HELO_FIRST)
        {

            std::string strFirst = "250 OK\r\n";
            std::string strAuth1 = "250-AUTH LOGIN PLAIN\r\n";
            std::string strAuth2 = "250-AUTH=LOGIN PLAIN\r\n";
            std::string str8BitTime = "250 8BITMIME\r\n";
            return strFirst +strAuth1 + str8BitTime;
        }
        if (step == Smtp_Server_Step_t::SMTP_RECV_AUTH_LOGIN_REQ)
        {
            std::string strAuth2 = "334 dXNlcm5hbWU6\r\n";
            return strAuth2;
        }
        if (step == Smtp_Server_Step_t::SMTP_RECV_PASS_WORD_REQ)
        {
            std::string strAuth2 = "334 UGFzc3dvcmQ6\r\n";
            return strAuth2;
        }
        if(step == Smtp_Server_Step_t::SMTP_NAME_PASS_VERIFY)
        {
            std::string strAuth2  = "235 Authentication successful\r\n";
            return strAuth2;
        }
        if(step == Smtp_Server_Step_t::SMTP_RECV_MAIL_FROM_REQ)
        {
            std::string strAuth2  = "250 Mail Ok\r\n";
            return strAuth2;
        }
        if(step == Smtp_Server_Step_t::SMTP_RECV_RCPT_TO_REQ)
        {
            std::string strAuth2  = "250 Mail Ok\r\n";
            return strAuth2;
        }
        if(step == Smtp_Server_Step_t::SMTP_RECV_DATA_REQ)
        {
            std::string strAuth2  = "354 End data with  <CR> <LF>. <CR> <LF>\r\n";
            return strAuth2;
        }
        return "";
    }
    std::string CSmtpServerProtoHandler::GetResponse()
    {
        std::string strRsp = m_strResponse;
        m_strResponse.clear();
        return strRsp;
    }
    std::string CSmtpServerProtoHandler::UserName()
    {
        return "";
    }
    std::string CSmtpServerProtoHandler::GetPassowrd()
    {
        return "";
    }
    CSmtpServerProtoHandler::~CSmtpServerProtoHandler()
    {
        std::cout<<"EMAIL Data:  "<<m_emailData<<std::endl;
    }


    bool CSmtpServerProtoHandler::OnEhloReq(const std::string strReq)
    {
        if(strReq.find("\r\n") != std::string::npos)
        {
            if(strReq.find("HELO ") != std::string::npos)
            {
                return true;
            }
        }
        if(strReq.find("\r\n") != std::string::npos)
        {
            if(strReq.find("EHLO") != std::string::npos)
            {
                return true;
            }
        }
        return false;
    }
    bool CSmtpServerProtoHandler::OnAuthLoginReq(const std::string strReq)
    {
        if(strReq.find("\r\n") != std::string::npos)
        {
            if(strReq.find("AUTH LOGIN") != std::string::npos)
            {
                return true;
            }
        }
        if(strReq.find("\r\n") != std::string::npos)
        {
            if(strReq.find("AUTH PLAIN") != std::string::npos)
            {
                m_strResponse="235 2.7.0 Authentication successful\r\n";
                m_step = Smtp_Server_Step_t::SMTP_RECV_MAIL_FROM_REQ;
                return false;
            }
        }
         if(strReq.find("\r\n") != std::string::npos)
        {
            if(strReq.find("AUTH CRAM-MD5") != std::string::npos)
            {
                m_strResponse="NOT Support\n";
                m_step = Smtp_Server_Step_t::SMTP_END;
                return false;
            }
        }
        return false;
    }
    bool CSmtpServerProtoHandler::OnPasswordReq(const std::string strReq)
    {
        if(strReq.find("\r\n") != std::string::npos)
        {
            m_strUserName = CProtoUtil::Base64Decode(strReq);
            return true;
        }
        return false;
    }
    bool CSmtpServerProtoHandler::OnNamePassVerifyReq(const std::string strReq)
    {
        m_strPassword = CProtoUtil::Base64Decode(strReq);
        if(m_db)
        {
            m_strUserAddr = tiny_email::CProtoUtil::CreateUserAddrFromNameAndDomain(m_strUserName,m_strEmailDomain);
            if(m_db->IsPasswordRight(m_strUserAddr,m_strPassword))
            {
                LOG_INFO(g_log,"User {} verify Passed",m_strUserAddr);
                m_strResponse = "235 Authentication successful\r\n";
            }
            else
            {
                LOG_ERROR(g_log,"UserName or password not right UserReq:{} {} {}",m_strUserAddr,m_strPassword,__LINE__);
                m_strResponse = "";
                m_step = Smtp_Server_Step_t::SMTP_END;
            }
        }
        return true;
    }
    bool CSmtpServerProtoHandler::OnMailFromReq(const std::string strReq)
    {
        m_strResponse = "235 Authentication successful\r\n";
        return true;
    }
    bool CSmtpServerProtoHandler::OnRcptToReq(const std::string strReq)
    {
        return true;
    }
    bool CSmtpServerProtoHandler::OnDataReq(const std::string strReq)
    {
        return true;
    }
    bool CSmtpServerProtoHandler::OnData(const std::string strReq)
    {
        m_emailData += strReq;
       
        if(m_emailData.find("\r\n.\r\n") != std::string::npos)
        {
            email_info_t email;
            bool bRet = ParseEmailForSmtp(m_emailData,email);
            if(bRet)
            {
                if(m_db)
                {
                    email.emailTime_ = CProtoUtil::Now();
                    m_db->SaveSendMailInfo(email);
                    LOG_INFO(g_log,"User {} email save succeed",m_strUserName);
                }
            }
            m_bFinished = true;
            return true;
        }
        else
        {
            return false;
        }
    }
}