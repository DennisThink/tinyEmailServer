#include "pop3_server_handler.h"
#include "CPop3ProtoCmd.h"
#include "ProtoUtil.h"
#include "SqliteDataBase.h"
#include "LogUtil.h"
#include <iostream>
namespace tiny_email
{
    //static auto g_log = GetLogger();
    CPop3ServerProtoHandler::CPop3ServerProtoHandler(CDataBaseInterface_SHARED_PTR dbPtr,const std::string strDomainName):CEmailServerProtoInterface(dbPtr, strDomainName)
    {
        m_step = POP3_SERVER_STEP_t::POP3_STEP_SERVER_ON_CONNECT;
        m_strResponse = GetNextStepCmd(m_step);
        m_bFinished = false;
        //m_log=std::make_shared<LogConsole>();
    }

    bool (CPop3ServerProtoHandler::*pFunc3)(const std::string &strReq);
    struct Pop3ServerStepElem
    {
        POP3_SERVER_STEP_t curStep_;
        POP3_SERVER_STEP_t nextStep_;
        POP3_CMD_t cmdCode_;
        decltype(pFunc3) callback_;
    };

  
    bool CPop3ServerProtoHandler::OnClientReq(const std::string strValue)
    {
        static Pop3ServerStepElem stepArrayPop[] = {
        {POP3_SERVER_STEP_t::POP3_STEP_SERVER_ON_CONNECT, POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_USER_NAME_OK, POP3_CMD_t::POP3_CMD_USER_NAME, &CPop3ServerProtoHandler::OnUser},
        {POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_USER_NAME_OK, POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK, POP3_CMD_t::POP3_CMD_PASS_WORD, &CPop3ServerProtoHandler::OnPassword},
        {POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK, POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK, POP3_CMD_t::POP3_CMD_STAT, &CPop3ServerProtoHandler::OnState},
        {POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK, POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK, POP3_CMD_t::POP3_CMD_RETR, &CPop3ServerProtoHandler::OnRetr},
        {POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK, POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK, POP3_CMD_t::POP3_CMD_LIST, &CPop3ServerProtoHandler::OnList},
        {POP3_SERVER_STEP_t::POP3_STEP_SERVER_ON_CONNECT, POP3_SERVER_STEP_t::POP3_STEP_SERVER_ON_CONNECT, POP3_CMD_t::POP3_CMD_CAPA, &CPop3ServerProtoHandler::OnCapa},
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
                    if (reqCmd.GetCode() == POP3_CMD_t::POP3_CMD_QUIT)
                    {
                        m_bFinished = true;
                    }
                    return true;
                }
            }
        }
        return false;
    }

    bool CPop3ServerProtoHandler::OnNoOp(const std::string &strRecv)
    {
        return false;
    }

    bool CPop3ServerProtoHandler::IsFinished()
    {
        return false;
    }
    std::string CPop3ServerProtoHandler::GetResponse()
    {
        std::string strRsp = m_strResponse;
        m_strResponse.clear();
        return strRsp;
    }
    CPop3ServerProtoHandler::~CPop3ServerProtoHandler()
    {
        std::cout << "EMAIL Data:  " << m_emailData << std::endl;
    }

    bool CPop3ServerProtoHandler::OnState(const std::string &strRecv)
    {
        m_strResponse = "+OK 0\r\n";
        return true;
    }
    bool CPop3ServerProtoHandler::OnRetr(const std::string &strRecv)
    {
        //LOG_INFO(g_log,"RETR:{} {}",strRecv,__LINE__);
        CPop3ProtoReqCmd cmd;
        PARSE_POP3_RESULT result = CPop3ProtoReqCmd::FromString(strRecv, cmd);
        if (PARSE_POP3_RESULT::PARSE_POP3_SUCCEED == result)
        {
            std::size_t nIndex = std::atoll(cmd.GetMsg().c_str());
            if(nIndex >= m_emailArray.size())
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
        //LOG_INFO(g_log,"RETR Rsp:{}",m_strResponse);
        return true;
    }

    bool CPop3ServerProtoHandler::OnList(const std::string &strRecv)
    {
        if (m_db)
        {
            //LOG_INFO(g_log,"Pop3 Handle User:  {}",m_strUserAddr);
            m_emailArray.clear();
            m_db->GetRecvMailInfo(m_strUserAddr, m_emailArray);
        }
        else
        {
            //LOG_ERROR(g_log,"Pop3 Handle User:  {}",m_strUserAddr);
        }
        if (!m_emailArray.empty())
        {
            std::size_t count = m_emailArray.size();
            m_strResponse = "+OK "+std::to_string(count)+ " 102\r\n";
            for(std::size_t i = 1; i <= count ; i++)
            {
                m_strResponse+=std::to_string(i)+" 102\r\n";
                //LOG_INFO(g_log,"Pop3 Handle User:  {} Time:{}",m_strUserAddr,m_emailArray[i-1].toString());
            }
            m_strResponse+=".\r\n";
        }
        else
        {
            m_strResponse = "+OK 0 .\r\n";
            //LOG_ERROR(g_log,"Pop3 Handle User:  {} No Email",m_strUserAddr);
        }
        return true;
    }

    //Todo: the ability should be modified,extend command, support later
    bool CPop3ServerProtoHandler::OnCapa(const std::string& strRecv)
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
    std::string CPop3ServerProtoHandler::GetPassWordOkSend()
    {
        return "";
    }
    std::string CPop3ServerProtoHandler::GetNextStepCmd(const POP3_SERVER_STEP_t &step)
    {
        switch (step)
        {
        case POP3_SERVER_STEP_t::POP3_STEP_SERVER_ON_CONNECT:
        {
            std::string strResponse = "+OK Welcome to " + m_strDomainName+ "\r\n";
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

    bool CPop3ServerProtoHandler::OnUser(const std::string &strUser)
    {
        //LOG_INFO(g_log,"USER:{} {}",strUser,__LINE__);
        CPop3ProtoReqCmd cmd;
        PARSE_POP3_RESULT result = CPop3ProtoReqCmd::FromString(strUser, cmd);
        if (PARSE_POP3_RESULT::PARSE_POP3_SUCCEED == result)
        {
            m_step = POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_USER_NAME_OK;
            //m_strUserName = CProtoUtil::CreateUserAddrFromNameAndDomain(CProtoUtil::Trim(cmd.GetMessage()),m_strEmailDomain);
            m_strUserName = CProtoUtil::Trim(cmd.GetMsg());
            //LOG_INFO(g_log,"User {} {}",m_strUserName, m_strDomainName);
            m_strUserAddr = CProtoUtil::CreateUserAddrFromNameAndDomain(m_strUserName, m_strDomainName);
            //LOG_INFO(g_log,"User: {} is login on Pop3 {}",m_strUserName,m_strUserAddr);
            return true;
        }
        else
        {
            m_step = POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_USER_NAME_BAD;
            return false;
        }
    }

    void CPop3ServerProtoHandler::MailInfoUpdate()
    {
        m_strEmailListDetail.clear();
        m_strEmailStateSummary.clear();
        m_emailArray.clear();
        if (m_db)
        {
            m_db->GetRecvMailInfo(m_strUserAddr, m_emailArray);
        }
        else
        {
            //LOG_ERROR(g_log, "Pop3 Handle User:  {}", m_strUserAddr);
        }
        if (!m_emailArray.empty())
        {
            std::size_t count = m_emailArray.size();
            std::size_t nSumSize = 0;
            std::string strDetail;
            for (std::size_t i = 0; i < count; i++)
            {
                nSumSize+= m_emailArray[i].emailBytes_;
                strDetail += std::to_string(i+1) + std::to_string(m_emailArray[i].emailBytes_)+ "\r\n";
                //LOG_INFO(g_log, "Pop3 Handle User:  {} Time:{}", m_strUserAddr, m_emailArray[i].toString());
            }
            m_strEmailListDetail = "+OK " + std::to_string(count) + " \r\n";
            m_strEmailStateSummary = m_strEmailListDetail;
            m_strEmailListDetail += strDetail;
            m_strEmailListDetail += ". \r\n";
        }
        else
        {
            m_strEmailListDetail = "+OK 0 \r\n";
            //LOG_ERROR(g_log, "Pop3 Handle User:  {} No Email", m_strUserAddr);
        }
    }
    bool CPop3ServerProtoHandler::OnPassword(const std::string &strPasswd)
    {
        CPop3ProtoReqCmd cmd;
        PARSE_POP3_RESULT result = CPop3ProtoReqCmd::FromString(strPasswd, cmd);
        if (PARSE_POP3_RESULT::PARSE_POP3_SUCCEED == result)
        {
            m_step = POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_OK;

            m_strPassword = CProtoUtil::Trim(cmd.GetMsg());
            //LOG_INFO(g_log,"USER:{} Pass:{} ",m_strUserAddr,m_strPassword);
            if (m_db && m_db->IsPasswordRight(m_strUserAddr, m_strPassword))
            {
                //LOG_INFO(g_log,"User: {} Verify Succeed",m_strUserAddr);
                MailInfoUpdate();
                m_strResponse = m_strEmailListDetail;
                return true;
            }
            else
            {
                //LOG_ERROR(g_log,"User: {} Verify Failed",m_strUserAddr);
                return false;
            }
        }
        else
        {
            m_step = POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_PASS_WORD_BAD;
            return false;
        }
    }

    bool CPop3ServerProtoHandler::OnRecv(const std::string &strRecv)
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
                return OnUser(cmd.GetMsg());
            }
            break;
            case POP3_CMD_t::POP3_CMD_PASS_WORD:
            {
                return OnPassword(cmd.GetMsg());
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
                return OnRetr(cmd.GetMsg());
                //m_step = POP3_SERVER_STEP_t::POP3_STEP_SERVER_SEND_RETR_OK;
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