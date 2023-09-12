#include "smtp_server_handler.h"
#include "CProtoCmd.h"
#include "ProtoUtil.h"
#include <iostream>
namespace tiny_email
{
    CSmtpServerHandler::CSmtpServerHandler()
    {
        m_strEmailDomain = "smtp.test.com";
        m_step = Smtp_Server_Step_t::SMTP_ON_CONNECT;
        m_strResponse = GetNextStepCmd(m_step);
        m_step = Smtp_Server_Step_t::SMTP_RECV_HELO_FIRST;
    }

    bool (CSmtpServerHandler::*pFunc)(const std::string strReq);
    struct SmtpServerStepElem
    {
        Smtp_Server_Step_t curStep;
        Smtp_Server_Step_t nextStep;
        decltype(pFunc)  callback_func;
    };
    struct SmtpServerStepElem stepArray[] =
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
    };

    bool CSmtpServerHandler::OnClientReq(const std::string strValue)
    {
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
        m_strResponse = "235 Authentication successful\r\n";
        m_strPassword = CProtoUtil::Base64Decode(strReq);
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
            return true;
        }
        else
        {
            return false;
        }
    }
}

/*
CServerSmtpProto::CServerSmtpProto(log_ptr_t log,CDataBaseInterface_SHARED_PTR ptr) : m_log(log),m_dataBase(ptr)
{
m_step = SERVER_SMTP_SEND_ON_CONNECT;
m_bAuthFinish = false;
m_strEmailDomain = "cppemail.com";
m_errorCmdCount = 0;
}
std::string CServerSmtpProto::GetSend()
{
if(m_step == SERVER_SMTP_EMAIL_DATA)
{
return "";
}
switch (m_step)
{
case SERVER_SMTP_SEND_HELO_HELO_FIRST:
{
    return "503 Error: send HELO/EHLO first\r\n";
}break;
case SERVER_SMTP_SEND_ON_CONNECT:
{
std::string strCode = "220 ";
std::string strTail = " Anti-spam GT for Coremail System\r\n";
return strCode + m_strEmailDomain + strTail;
}
break;
case SERVER_SMTP_SEND_250:
{
std::string strFirst = "250-mail\r\n";
std::string strAuth1 = "250-AUTH LOGIN PLAIN\r\n";
std::string strAuth2 = "250-AUTH=LOGIN PLAIN\r\n";
std::string str8BitTime = "250 8BITMIME\r\n";
return strFirst + strAuth1 + strAuth2 + str8BitTime;
}
break;
case SERVER_SMTP_SEND_BYE_221:
{
return "221 Bye\r\n";
}break;
case SERVER_SMTP_SEND_USER_NAME_334:
{
std::string strCode = "334 ";
std::string userName = CProtoUtil::Base64Encode("UserName");
return strCode + userName + "\r\n";
}
break;
case SERVER_SMTP_SEND_PASS_WORD_334:
{
std::string strCode = "334 ";
std::string password = CProtoUtil::Base64Encode("Password");
return strCode + password + "\r\n";
}
break;
case SERVER_SMTP_SEND_AUTH_SUCCESS:
{
return "235 auth successfully\r\n";
}
break;
case SEND_USER_NAME:
{
return CProtoUtil::Base64Encode("dennismi@163.com") + "\r\n";
}
break;
case SEND_PASSWORD:
{
return CProtoUtil::Base64Encode("srpPr9MXwp285Su") + "\r\n";
}
break;
case SEND_MAIL_FROM:
{
return "MAIL FROM: <dennismi@163.com>\r\n";
}
break;
case SEND_MAIL_TO:
{
return "RCPT TO: <dennismi1024@gmail.com>\r\n";
}
break;
case SERVER_SMTP_SEND_AUTH_FAILED_535:
{
return "535 Error: authentication failed\r\n";
}
break;
case SERVER_SMTP_SEND_CMD_NOT_IMPLEMENT_502:
{
m_errorCmdCount++;
return "502 Error: command not implemented\r\n";
}
break;
case SERVER_SMTP_SEND_TOO_MANY_ERROR_CMDS_421:
{
return "421 Too many error commands\r\n";
}
break;
case SERVER_SMTP_SEND_MAIL_FROM_250:
{
return "250 MAIL FROM Succeed\r\n";
}break;
case SERVER_SMTP_SEND_RCPT_TO_250:
{
return "250 RCPT TO Succeed\r\n";
}break;
case SERVER_SMTP_SEND_QUIT:
{
return "221 Bye\r\n";
}break;
case SERVER_SMTP_SEND_DATA:
{
m_step = SERVER_SMTP_EMAIL_DATA;
return "354 Enter mail, end with \".\" on a line by itself\r\n";
}break;
case SERVER_SMTP_EMAIL_DATA:
{
return "";
}break;
case SERVER_SMTP_EMAIL_FINISH:
{
return "250 Mail OK queued as smtp11,D8CowAD3q0+vOSBc4a4JEg--.44272S2 1545615792\r\n";
}break;
default:
{
return "";
}
break;
}
}








bool CServerSmtpProto::OnRecv(const std::string &strRecv)
{
CSmtpProtoReqCmd cmd;
PARSE_SMTP_RESULT result = CSmtpProtoReqCmd::FromString(strRecv,cmd);
std::cout<<static_cast<int>(result)<<static_cast<int>(cmd.GetCode())<<std::endl;
if(PARSE_SMTP_RESULT::PARSE_SMTP_SUCCEED == result)
{
if(cmd.GetCode() == SMTP_CMD_t::SMTP_CMD_HELO_EHLO)
{
    m_step = SERVER_SMTP_SEND_250;
    return true;
}

if(cmd.GetCode() == SMTP_CMD_t::SMTP_CMD_AUTH)
{
    return OnAuthRecv();
}

if(cmd.GetCode() == SMTP_CMD_t::SMTP_CMD_MAIL_FROM)
{
    return OnMailFromRecv();
}

if(cmd.GetCode() == SMTP_CMD_t::SMTP_CMD_RECP_TO)
{
    return OnMailRecpToRecv();
}

if(cmd.GetCode() == SMTP_CMD_t::SMTP_CMD_QUIT)
{
    m_step = SERVER_SMTP_SEND_QUIT;
    return true;
}

if(cmd.GetCode() == SMTP_CMD_t::SMTP_CMD_DATA)
{
    m_step = SERVER_SMTP_SEND_DATA;
    return true;
}

if(cmd.GetCode() == SMTP_CMD_t::SMTP_CMD_RSET)
{
    return OnRsetRecv();
}
}



if (SERVER_SMTP_SEND_USER_NAME_334 == m_step)
{
std::string strUserName = CProtoUtil::Base64Decode(strRecv);
if (m_dataBase->IsUserExist(strUserName))
{
    m_log->info("UserName: {} Good", strUserName);
    m_step = SERVER_SMTP_SEND_PASS_WORD_334;
}
else
{
    m_log->warn("UserName: {} Bad", strUserName);
    m_step = SERVER_SMTP_SEND_AUTH_FAILED_535;
}

return true;
}

if (SERVER_SMTP_SEND_PASS_WORD_334 == m_step)
{
std::cout << "Password:  " << CProtoUtil::Base64Decode(strRecv) << std::endl;
m_step = SERVER_SMTP_SEND_AUTH_SUCCESS;
return true;
}

if(m_step == SERVER_SMTP_EMAIL_DATA)
{
m_emailData = m_emailData.append(strRecv);
if(m_emailData.find("\r\n.\r\n") != std::string::npos)
{
    m_step = SERVER_SMTP_EMAIL_FINISH;
    email_info_t email;
    if(ParseEmailFromRecv(m_emailData,email))
    {
        m_dataBase->SaveSendMailInfo(email);
        m_log->info("Save Email");
        m_emailData.clear();
    }
    m_log->info("EMAILDATA:\r\n {}",m_emailData);
}
return true;
}
m_step = SERVER_SMTP_SEND_CMD_NOT_IMPLEMENT_502;
if (m_errorCmdCount > 3)
{
m_step = SERVER_SMTP_SEND_TOO_MANY_ERROR_CMDS_421;
}
return true;
}

bool CServerSmtpProto::OnRsetRecv()
{
if(m_step == SERVER_SMTP_SEND_ON_CONNECT )
{
m_step = SERVER_SMTP_SEND_HELO_HELO_FIRST;
}
return true;
}

bool CServerSmtpProto::SendMail()
{
if (IsAuthFinished() && m_step == AUTH_SUCCESS)
{
m_step = SEND_MAIL_FROM;
}
return true;
}
*/