#include "pop3_server_handler.h"
#include "CProtoCmd.h"
#include "CPop3ProtoCmd.h"
#include "ProtoUtil.h"
#include "SqliteDataBase.h"
#include <iostream>
namespace tiny_email
{
    CPop3ServerHandler::CPop3ServerHandler(CDataBaseInterface_SHARED_PTR dbPtr)
    {
        m_strEmailDomain = "pop.test.com";
        m_step = POP3_SERVER_STEP_t::POP3_STEP_SERVER_ON_CONNECT;
        m_strResponse = GetNextStepCmd(m_step);
        m_db = dbPtr;
    }

    bool (CPop3ServerHandler::*pFunc3)(const std::string &strReq);
    struct Pop3ServerStepElem
    {
        POP3_SERVER_STEP_t curStep_;
        POP3_SERVER_STEP_t nextStep_;
        POP3_CMD_t cmdCode_;
        decltype(pFunc3) callback_;
    };

  
    bool CPop3ServerHandler::OnClientReq(const std::string strValue)
    {
        static Pop3ServerStepElem stepArrayPop[] = {
        {POP3_SERVER_STEP_t::POP3_STEP_SERVER_ON_CONNECT, POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_USER_NAME_OK, POP3_CMD_t::POP3_CMD_USER_NAME, &CPop3ServerHandler::OnUser},
        {POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_USER_NAME_OK, POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK, POP3_CMD_t::POP3_CMD_PASS_WORD, &CPop3ServerHandler::OnPassword},
        {POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK, POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK, POP3_CMD_t::POP3_CMD_STAT, &CPop3ServerHandler::OnState},
        {POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK, POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK, POP3_CMD_t::POP3_CMD_RETR, &CPop3ServerHandler::OnNoOp},
        {POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK, POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK, POP3_CMD_t::POP3_CMD_LIST, &CPop3ServerHandler::OnList},
        {POP3_SERVER_STEP_t::POP3_STEP_SERVER_ON_CONNECT, POP3_SERVER_STEP_t::POP3_STEP_SERVER_ON_CONNECT, POP3_CMD_t::POP3_CMD_CAPA, &CPop3ServerHandler::OnCapa},
        };
        CPop3ProtoReqCmd reqCmd;
        PARSE_POP3_RESULT result = CPop3ProtoReqCmd::FromString(strValue, reqCmd);
        std::cout << "Result: " << static_cast<int>(result) << " Code: " << static_cast<int>(reqCmd.GetCode()) << " Msg: " << reqCmd.GetMessage() << std::endl;
        if (result == PARSE_POP3_RESULT::PARSE_POP3_SUCCEED)
        {
            for (std::size_t i = 0; i < sizeof(stepArrayPop) / sizeof(stepArrayPop[0]); i++)
            {
                if (m_step == stepArrayPop[i].curStep_ && reqCmd.GetCode() == stepArrayPop[i].cmdCode_)
                {
                    bool bRet = (this->*stepArrayPop[i].callback_)(strValue);
                    if (m_strResponse.empty())
                    {
                        m_strResponse = GetNextStepCmd(m_step);
                    }
                    m_step = stepArrayPop[i].nextStep_;
                    return true;
                }
            }
        }
        return false;
    }

    bool CPop3ServerHandler::OnNoOp(const std::string &strRecv)
    {
        return false;
    }
    std::string CPop3ServerHandler::GetResponse()
    {
        std::string strRsp = m_strResponse;
        m_strResponse.clear();
        return strRsp;
    }
    CPop3ServerHandler::~CPop3ServerHandler()
    {
        std::cout << "EMAIL Data:  " << m_emailData << std::endl;
    }

    bool CPop3ServerHandler::OnState(const std::string &strRecv)
    {
        m_strResponse = "+OK 1 102\r\n";
        return true;
    }
    bool CPop3ServerHandler::OnRetr(const std::string &strRecv)
    {
        m_strResponse = R"(Date: Tue, 29 Aug 2023 07:08:51 +0800 (CST)
From: test2@test.com
To: test1@test.com
Message-ID: <2053286275.7139059.1693264131639>
Subject: test tiny email
MIME-Version: 1.0
Content-Type: text/plain; charset=utf-8

this is the test email from tiny email server

.
)";
        return true;
    }

    bool CPop3ServerHandler::OnList(const std::string &strRecv)
    {
        if (m_db)
        {
            m_emailArray.clear();
            m_db->GetRecvMailInfo(m_strUserName, m_emailArray);
        }
        if (!m_emailArray.empty())
        {
            std::size_t count = m_emailArray.size();
            m_strResponse = "+OK "+std::to_string(count)+ "102\r\n";
            for(std::size_t i = 1; i < count ; i++)
            {
                m_strResponse+=std::to_string(i)+" 102\r\n";
            }
            m_strResponse+=".\r\n";
        }
        return true;
    }

    bool CPop3ServerHandler::OnCapa(const std::string& strRecv)
    {
        m_strResponse=R"(+OK Capability list follows
LANG
TOP
USER
UTF8
STLS
PIPELINING
SASL(PLAIN XOAUTH2)
ID
UIDL
SASL PLAIN
STLS
.

)";
        return true;
    }
    std::string CPop3ServerHandler::GetPassWordOkSend()
    {
        return "";
    }
    std::string CPop3ServerHandler::GetNextStepCmd(const POP3_SERVER_STEP_t &step)
    {
        switch (step)
        {
        case POP3_SERVER_STEP_t::POP3_STEP_SERVER_ON_CONNECT:
        {
            return "+OK Welcome to pop.test.com\r\n";
        }
        break;
        case POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_USER_NAME_OK:
        {
            return "+OK cpp mail user name\r\n";
        }
        break;
        case POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_USER_NAME_BAD:
        {
            return "-ERR core mail user name(cpp email)\r\n";
        }
        break;
        case POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK:
        {
            return "+OK 1 message(s) [102 byte(s)]\r\n";
        }
        break;
        case POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_BAD:
        {
            return "-ERR core mail pass word(cpp email)\r\n";
        }
        break;
        case POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_STAT_OK:
        {
            return "+OK 8 88108\r\n";
        }
        break;
        case POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_UIDL_OK:
        {
            return "+OK core mail pass word\r\n";
        }
        break;
        case POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_RETR_OK:
        {
            return "+OK core mail pass word\r\n";
        }
        break;
        case POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_NOOP_OK:
        {
            return "+OK core mail no op\r\n";
        }
        break;
        case POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_DELE_OK:
        {
            return "+OK core mail dele\r\n";
        }
        break;
        default:
        {
            return "";
        }
        break;
        }
    }

    bool CPop3ServerHandler::OnUser(const std::string &strUser)
    {
        if (true)
        {
            m_step = POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_USER_NAME_OK;
            CPop3ProtoReqCmd reqCmd;
            PARSE_POP3_RESULT result = CPop3ProtoReqCmd::FromString(strUser, reqCmd);
            if (result == PARSE_POP3_RESULT::PARSE_POP3_SUCCEED)
            {
                m_strUserName = reqCmd.GetMessage();
            }
            return true;
        }
        else
        {
            m_step = POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_USER_NAME_BAD;
            return false;
        }
    }

    bool CPop3ServerHandler::OnPassword(const std::string &strPasswd)
    {
        if (true)
        {
            m_step = POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK;
            CPop3ProtoReqCmd reqCmd;
            PARSE_POP3_RESULT result = CPop3ProtoReqCmd::FromString(strPasswd, reqCmd);
            if (result == PARSE_POP3_RESULT::PARSE_POP3_SUCCEED)
            {
                m_strPassword = reqCmd.GetMessage();
            }
            if (m_db && m_db->IsPasswordRight(m_strUserName, m_strPassword))
            {
                std::cout << "User:  " << m_strUserName << "  Password Match" << std::endl;
                return true;
            }
            else
            {
                std::cerr << "User:  " << m_strUserName << "Pass:" <<m_strPassword<< "  Password Not Match" << std::endl;
                return false;
            }
        }
        else
        {
            m_step = POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_BAD;

            return false;
        }
    }

    bool CPop3ServerHandler::OnRecv(const std::string &strRecv)
    {
        if (strRecv.empty())
        {
            return false;
        }
        CPop3ProtoReqCmd cmd;
        PARSE_POP3_RESULT result = CPop3ProtoReqCmd::FromString(strRecv, cmd);
        if (PARSE_POP3_RESULT::PARSE_POP3_SUCCEED == result)
        {
            switch (cmd.GetCode())
            {
            case POP3_CMD_t::POP3_CMD_USER_NAME:
            {
                return OnUser(cmd.GetMessage());
            }
            break;
            case POP3_CMD_t::POP3_CMD_PASS_WORD:
            {
                return OnPassword(cmd.GetMessage());
            }
            break;
            case POP3_CMD_t::POP3_CMD_STAT:
            {
                m_step = POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_STAT_OK;
            }
            break;
            case POP3_CMD_t::POP3_CMD_UIDL:
            {
                m_step = POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_UIDL_OK;
            }
            break;
            case POP3_CMD_t::POP3_CMD_RETR:
            {
                m_step = POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_RETR_OK;
            }
            break;
            case POP3_CMD_t::POP3_CMD_NOOP:
            {
                m_step = POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_NOOP_OK;
            }
            break;
            case POP3_CMD_t::POP3_CMD_DELE:
            {
                m_step = POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_DELE_OK;
            }
            break;
            default:
            {
            }
            break;
            }
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