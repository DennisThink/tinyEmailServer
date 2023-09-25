#include "pop3_server_handler.h"
#include "CProtoCmd.h"
#include "CPop3ProtoCmd.h"
#include "ProtoUtil.h"
#include "SqliteDataBase.h"
#include "LogUtil.h"
#include <iostream>
namespace tiny_email
{
    static auto g_log = GetLogger();
    CPop3ServerHandler::CPop3ServerHandler(CDataBaseInterface_SHARED_PTR dbPtr,const std::string strDomainName)
    {
        m_strEmailDomain = strDomainName;
        m_step = POP3_SERVER_STEP_t::POP3_STEP_SERVER_ON_CONNECT;
        m_strResponse = GetNextStepCmd(m_step);
        m_db = dbPtr;
        //m_log=std::make_shared<LogConsole>();
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
        {POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK, POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK, POP3_CMD_t::POP3_CMD_RETR, &CPop3ServerHandler::OnRetr},
        {POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK, POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK, POP3_CMD_t::POP3_CMD_LIST, &CPop3ServerHandler::OnList},
        {POP3_SERVER_STEP_t::POP3_STEP_SERVER_ON_CONNECT, POP3_SERVER_STEP_t::POP3_STEP_SERVER_ON_CONNECT, POP3_CMD_t::POP3_CMD_CAPA, &CPop3ServerHandler::OnCapa},
        };
        CPop3ProtoReqCmd reqCmd;
        PARSE_POP3_RESULT result = CPop3ProtoReqCmd::FromString(strValue, reqCmd);
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
        LOG_INFO(g_log,"RETR:{} {}",strRecv,__LINE__);
        CPop3ProtoReqCmd cmd;
        PARSE_POP3_RESULT result = CPop3ProtoReqCmd::FromString(strRecv, cmd);
        if (PARSE_POP3_RESULT::PARSE_POP3_SUCCEED == result)
        {
            std::size_t nIndex = std::atoll(cmd.GetMessage().c_str());
            if(nIndex > m_emailArray.size())
            {
                nIndex = m_emailArray.size()-1;
            }
            else
            {

            }
            std::string strRsp;
            if(Pop3RspFromEmail(m_emailArray[nIndex],strRsp))
            {
                m_strResponse = strRsp;
            }
        }
        LOG_INFO(g_log,"RETR Rsp:{}",m_strResponse);
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
            m_strResponse = "+OK "+std::to_string(count)+ " 102\r\n";
            for(std::size_t i = 1; i <= count ; i++)
            {
                m_strResponse+=std::to_string(i)+" 102\r\n";
            }
            m_strResponse+=".\r\n";
        }
        return true;
    }

    //Todo: the ability should be modified,extend command, support later
    bool CPop3ServerHandler::OnCapa(const std::string& strRecv)
    {
        m_strResponse="-ERR\r\n";
        return true;
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
            std::string strResponse = "+OK Welcome to " + m_strEmailDomain+ "\r\n";
            return strResponse;
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
        LOG_INFO(g_log,"USER:{} {}",strUser,__LINE__);
        CPop3ProtoReqCmd cmd;
        PARSE_POP3_RESULT result = CPop3ProtoReqCmd::FromString(strUser, cmd);
        if (PARSE_POP3_RESULT::PARSE_POP3_SUCCEED == result)
        {
            m_step = POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_USER_NAME_OK;
            m_strUserName = CProtoUtil::CreateUserAddrFromNameAndDomain(CProtoUtil::Trim(cmd.GetMessage()),m_strEmailDomain);
            LOG_INFO(g_log,"User: {} is login on Pop3",m_strUserName);
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
        CPop3ProtoReqCmd cmd;
        PARSE_POP3_RESULT result = CPop3ProtoReqCmd::FromString(strPasswd, cmd);
        if (PARSE_POP3_RESULT::PARSE_POP3_SUCCEED == result)
        {
            m_step = POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK;

            m_strPassword = CProtoUtil::Trim(cmd.GetMessage());
            LOG_INFO(g_log,"USER:{} Pass:{} ",m_strUserName,m_strPassword);
            if (m_db && m_db->IsPasswordRight(m_strUserName, m_strPassword))
            {
                LOG_INFO(g_log,"User: {} Verify Succeed",m_strUserName);
                return true;
            }
            else
            {
                LOG_ERROR(g_log,"User: {} Verify Failed",m_strUserName);
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