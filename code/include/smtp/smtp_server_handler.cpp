#include "smtp_server_handler.h"
#include "SqliteDataBase.h"
#include "CProtoCmd.h"
#include "ProtoUtil.h"
#include <iostream>
namespace tiny_email
{
    CSmtpServerHandler::CSmtpServerHandler(CDataBaseInterface_SHARED_PTR dbPtr)
    {
        m_strEmailDomain = "smtp.test.com";
        m_step = Smtp_Server_Step_t::SMTP_ON_CONNECT;
        m_strResponse = GetNextStepCmd(m_step);
        m_step = Smtp_Server_Step_t::SMTP_RECV_HELO_FIRST;
        m_db = dbPtr;
    }

    bool (CSmtpServerHandler::*pFunc)(const std::string strReq);
    struct SmtpServerStepElem
    {
        Smtp_Server_Step_t curStep;
        Smtp_Server_Step_t nextStep;
        decltype(pFunc)  callback_func;
    };
 
    bool CSmtpServerHandler::OnClientReq(const std::string strValue)
    {
        static struct SmtpServerStepElem stepArray[] =
        {
            {Smtp_Server_Step_t::SMTP_BEGIN, Smtp_Server_Step_t::SMTP_ON_CONNECT,&CSmtpServerHandler::OnEhloReq},
            {Smtp_Server_Step_t::SMTP_ON_CONNECT, Smtp_Server_Step_t::SMTP_RECV_HELO_FIRST,&CSmtpServerHandler::OnEhloReq},
            {Smtp_Server_Step_t::SMTP_RECV_HELO_FIRST, Smtp_Server_Step_t::SMTP_RECV_AUTH_LOGIN_REQ,&CSmtpServerHandler::OnEhloReq},
            {Smtp_Server_Step_t::SMTP_RECV_AUTH_LOGIN_REQ, Smtp_Server_Step_t::SMTP_RECV_PASS_WORD_REQ,&CSmtpServerHandler::OnAuthLoginReq},
            {Smtp_Server_Step_t::SMTP_RECV_PASS_WORD_REQ, Smtp_Server_Step_t::SMTP_NAME_PASS_VERIFY,&CSmtpServerHandler::OnPasswordReq},
            {Smtp_Server_Step_t::SMTP_NAME_PASS_VERIFY, Smtp_Server_Step_t::SMTP_RECV_MAIL_FROM_REQ,&CSmtpServerHandler::OnNamePassVerifyReq},
            {Smtp_Server_Step_t::SMTP_RECV_MAIL_FROM_REQ, Smtp_Server_Step_t::SMTP_RECV_RCPT_TO_REQ,&CSmtpServerHandler::OnMailFromReq},
            {Smtp_Server_Step_t::SMTP_RECV_RCPT_TO_REQ, Smtp_Server_Step_t::SMTP_RECV_DATA_REQ,&CSmtpServerHandler::OnRcptToReq},
            {Smtp_Server_Step_t::SMTP_RECV_DATA_REQ, Smtp_Server_Step_t::SMTP_RECV_EMAIL_DATA,&CSmtpServerHandler::OnDataReq},
            {Smtp_Server_Step_t::SMTP_RECV_EMAIL_DATA, Smtp_Server_Step_t::SMTP_RECV_EMAIL_DATA,&CSmtpServerHandler::OnData},
            {Smtp_Server_Step_t::SMTP_END,Smtp_Server_Step_t::SMTP_END,&CSmtpServerHandler::OnClientReq},
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
                   
                }
            }
        }
        return false;
    }
    std::string CSmtpServerHandler::GetNextStepCmd(const Smtp_Server_Step_t &step)
    {
        if (step == Smtp_Server_Step_t::SMTP_ON_CONNECT)
        {
            std::string strCode = "220 ";
            std::string strTail = " Anti-spam GT for Coremail System\r\n";
            return strCode + m_strEmailDomain + strTail;
        }
        if (step == Smtp_Server_Step_t::SMTP_RECV_HELO_FIRST)
        {
            std::string strFirst = "250-mail\r\n";
            std::string strAuth1 = "250-AUTH LOGIN PLAIN\r\n";
            std::string strAuth2 = "250-AUTH=LOGIN PLAIN\r\n";
            std::string str8BitTime = "250 8BITMIME\r\n";
            return strFirst + strAuth1 + strAuth2 + str8BitTime;
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
    std::string CSmtpServerHandler::GetResponse()
    {
        std::string strRsp = m_strResponse;
        m_strResponse.clear();
        return strRsp;
    }
    std::string CSmtpServerHandler::GetUserName()
    {
        return "";
    }
    std::string CSmtpServerHandler::GetPassowrd()
    {
        return "";
    }
    CSmtpServerHandler::~CSmtpServerHandler()
    {
        std::cout<<"EMAIL Data:  "<<m_emailData<<std::endl;
    }


    bool CSmtpServerHandler::OnEhloReq(const std::string strReq)
    {
        if(strReq.find("\r\n") != std::string::npos)
        {
            if(strReq.find("HELO ") != std::string::npos && strReq.find(m_strEmailDomain) != std::string::npos)
            {
                std::cout <<__LINE__<<"   strReq:    "<<   strReq <<std::endl;
                return true;
            }
        }
        return false;
    }
    bool CSmtpServerHandler::OnAuthLoginReq(const std::string strReq)
    {
        if(strReq.find("\r\n") != std::string::npos)
        {
            if(strReq.find("AUTH LOGIN") != std::string::npos)
            {
                std::cout <<__LINE__<<"   strReq:    "<<   strReq <<std::endl;
                return true;
            }
        }
        return false;
    }
    bool CSmtpServerHandler::OnPasswordReq(const std::string strReq)
    {
        if(strReq.find("\r\n") != std::string::npos)
        {
            m_strUserName = CProtoUtil::Base64Decode(strReq);
            std::cout <<__LINE__<<"   strReq:    "<<   strReq <<"   UserName: "<<m_strUserName <<std::endl;
            return true;
        }
        return false;
    }
    bool CSmtpServerHandler::OnNamePassVerifyReq(const std::string strReq)
    {
        m_strPassword = CProtoUtil::Base64Decode(strReq);
        if(m_db)
        {
            if(m_db->IsPasswordRight(m_strUserName,m_strPassword))
            {
                m_strResponse = "235 Authentication successful\r\n";
            }
            else
            {
                std::cout<<"Error"<<std::endl;
                m_strResponse = "";
                m_step = Smtp_Server_Step_t::SMTP_END;
            }
        }
        std::cout <<__LINE__<<"   strReq:    "<<   strReq<<"   Password: "<<m_strPassword <<std::endl;
        return true;
    }
    bool CSmtpServerHandler::OnMailFromReq(const std::string strReq)
    {
        m_strResponse = "235 Authentication successful\r\n";
        std::cout <<__LINE__<<"   strReq:    "<<   strReq <<std::endl;
        return true;
    }
    bool CSmtpServerHandler::OnRcptToReq(const std::string strReq)
    {
        std::cout <<__LINE__<<"   strReq:    "<<   strReq <<std::endl;
        return true;
    }
    bool CSmtpServerHandler::OnDataReq(const std::string strReq)
    {
        std::cout <<__LINE__<<"   strReq:    "<<   strReq <<std::endl;
        return true;
    }
    bool CSmtpServerHandler::OnData(const std::string strReq)
    {
        m_emailData += strReq;
       
        if(m_emailData.find("\r\n.\r\n") != std::string::npos)
        {
            std::cout <<__LINE__<<"   Email Data:    "<<   m_emailData <<std::endl;
            email_info_t email;
            bool bRet = ParseEmailForSmtp(m_emailData,email);
            if(bRet)
            {
                if(m_db)
                {
                    m_db->SaveSendMailInfo(email);
                }
            }
            return true;
        }
        else
        {
            return false;
        }
    }
}