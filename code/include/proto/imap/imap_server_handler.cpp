#include "imap_server_handler.h"
#include "CImapProtoCmd.h"
namespace tiny_email
{

    CImapServerProtoHandler::CImapServerProtoHandler(CDataBaseInterface_SHARED_PTR dbPtr, const std::string strDomainName):CEmailServerProtoHandlerInterface(dbPtr,strDomainName)
    {
        OnClientConnect("");
    }

    bool (CImapServerProtoHandler::*OnCmdFunction)(const std::string strValue);
    struct ImapElem_t {
        IMAP_SERVER_STEP_t curState;
        ImapCode_t         curCmdId;
        IMAP_SERVER_STEP_t nextState;
        decltype(OnCmdFunction)   callFunc;
    };

    static ImapElem_t imapArray[] =
    {
        {IMAP_STEP_SERVER_BEGIN,IMAP_CMD_CAPABILITY,IMAP_STEP_SERVER_NOT_LOGIN,&CImapServerProtoHandler::OnCapability},
        {IMAP_STEP_SERVER_NOT_LOGIN,IMAP_CMD_LOGIN,IMAP_STEP_SERVER_LOGIN_SUCCEED,&CImapServerProtoHandler::OnLogin}
    };

    const std::size_t IMAP_ARRAY_SIZE = sizeof(imapArray) / sizeof(imapArray[0]);
    bool  CImapServerProtoHandler::OnClientReq(const std::string strValue)
    {
        m_strReceive += strValue;
        if(IsCommandComplete(m_strReceive))
        { 
            std::string strCmd = m_strReceive;
            m_strReceive.clear();

           CImapProtoCmd cmd = CImapProtoCmd::FromString(strCmd);
           for (std::size_t i = 0; i < IMAP_ARRAY_SIZE; i++)
           {
               ImapElem_t& cur = imapArray[i];
               if (cur.curState == m_step && cur.curCmdId == cmd.GetCode())
               {
                   if ((this->*cur.callFunc)(strCmd))
                   {
                       m_step = cur.nextState;
                   }
                   else
                   {
                       m_step = IMAP_STEP_SERVER_END;
                   }
               }
           }
        }
        else
        {

        }
        return false;

    }

    bool CImapServerProtoHandler::IsFinished()
    {
        return false;
    }

    std::string  CImapServerProtoHandler::GetResponse()
    {
        std::string strResult = m_strResponse;
        m_strResponse.clear();
        return strResult;
    }
    CImapServerProtoHandler::~CImapServerProtoHandler()
    {

    }
    void CImapServerProtoHandler::MailInfoUpdate()
    {

    }

    bool CImapServerProtoHandler::IsCommandComplete(const std::string strValue)
    {
        if (strValue.find("\r\n") != std::string::npos)
        {
            return true;
        }
        return false;
    }
    std::string CImapServerProtoHandler::GetNextStepCmd(const POP3_SERVER_STEP_t& step)
    {
        if (step == POP3_SERVER_STEP_t::POP3_STEP_BEGIN)
        {
            return "";
        }
        else
        {
            return "";
        }
    }
    bool CImapServerProtoHandler::OnRecv(const std::string& strRecv)
    {
        if (strRecv.empty())
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    std::string CImapServerProtoHandler::GetPassWordOkSend()
    {
        return "";
    }

    bool  CImapServerProtoHandler::OnClientConnect(const std::string strValue)
    {
        m_strResponse = "* OK [] tiny email ready\r\n";
        return true;
    }
    bool  CImapServerProtoHandler::OnCapability(const std::string strValue)
    {
        m_strResponse = "* CAPABILITY IMAP4 IMAP4rev1 XLIST MOVE IDLE XAPPLEPUSHSERVICE AUTH=PLAIN AUTH=LOGIN AUTH=XOAUTH2 NAMESPACE CHILDREN ID UIDPLUS\r\n";
        m_strResponse += "A01 OK CAPABILITY Completed\r\n";
        return true;
    }
    bool  CImapServerProtoHandler::OnNoop(const std::string strValue)
    {
        m_strResponse = "A02 OK NOOP Completed\r\n";
        return true;
    }
    bool  CImapServerProtoHandler::OnSelect(const std::string strValue)
    {
        m_strResponse = "* 6 EXISTS\r\n";
        m_strResponse += "* 0 RECENT\r\n";
        m_strResponse += "* OK[UNSEEN 6]\r\n";
        m_strResponse += "* OK[UIDVALIDITY 1375069811] UID validity status\r\n";
        m_strResponse += "* OK[UIDNEXT 5258] Predicted next UID\r\n";
        m_strResponse += R"(* FLAGS(\Answered \Flagged \Deleted \Draft \Seen)\r\n)";
        m_strResponse += R"(* OK[PERMANENTFLAGS(\ * \Answered \Flagged \Deleted \Draft \Seen)] Permanent flags\r\n)";
        m_strResponse += " A04 OK[READ - WRITE] SELECT complete\r\n";
        return true;
    }
    bool  CImapServerProtoHandler::OnExamine(const std::string strValue)
    {
        m_strResponse = R"(*6 EXISTS\r\n)";
        m_strResponse += R"(* 0 RECENT\r\n)";
        m_strResponse += R"(* OK[UNSEEN 6]\r\n)";
        m_strResponse += R"(* OK[UIDVALIDITY 1375069811] UID validity status\r\n)";
        m_strResponse += R"(* OK[UIDNEXT 5258] Predicted next UID\r\n)";
        m_strResponse += R"(* FLAGS(\Answered \Flagged \Deleted \Draft \Seen)\r\n)";
        m_strResponse += R"(A05 OK[READ - ONLY] EXAMINE completent flags permitted\r\n)";
        return true;
    }
    bool  CImapServerProtoHandler::OnSearchAll(const std::string strValue)
    {
        m_strResponse = "* SEARCH 1 2 3 4 5 6 7\r\n";
        m_strResponse += "A07 OK SEARCH completed\r\n";
        return true;
    }
    bool  CImapServerProtoHandler::OnFetchOne(const std::string strValue)
    {
        m_strResponse = "* 7 FETCH (RFC822 {4017}\r\n";
        m_strResponse += "A08 OK FETCH Completed\r\n";
        return true;
    }
    bool  CImapServerProtoHandler::OnList(const std::string strValue)
    {
        m_strResponse = R"(* LIST(\NoSelect \HasChildren) "/" "&UXZO1mWHTvZZOQ-\"\r\n)";
        m_strResponse += R"(* LIST(\HasNoChildren) "/" "INBOX"\r\n)";
        m_strResponse += R"(* LIST(\HasNoChildren) "/" "Sent Messages"\r\n)";
        m_strResponse += R"(* LIST(\HasNoChildren) "/" "Drafts""\r\n)";
        m_strResponse += R"(* LIST(\HasNoChildren) "/" "Deleted Messages"\r\n)";
        m_strResponse += R"(* LIST(\HasNoChildren) "/" "Junk""\r\n)";
        m_strResponse += R"(* LIST(\HasNoChildren) "/" "&UXZO1mWHTvZZOQ-/QQ&kK5O9ouilgU-""\r\n)";
        m_strResponse += R"(* LIST(\HasNoChildren) "/" "&UXZO1mWHTvZZOQ-/1143456295@qq.com""\r\n)";
        m_strResponse += R"(* LIST(\HasNoChildren) "/" "&UXZO1mWHTvZZOQ-/dennismi@163.com""\r\n)";
        m_strResponse += R"(* LIST(\HasNoChildren) "/" "&UXZO1mWHTvZZOQ-/Github""\r\n)";
        m_strResponse += R"(* LIST(\HasNoChildren) "/" "&UXZO1mWHTvZZOQ-/&kdF7Gw-""\r\n)";
        m_strResponse += R"(* LIST(\HasNoChildren) "/" "&UXZO1mWHTvZZOQ-/HR""\r\n)";
        m_strResponse += R"( A06 OK LIST completedn) "/" "&UXZO1mWHTvZZOQ-/&lj+RzE6R-""\r\n)";
        return true;
    }
    bool  CImapServerProtoHandler::OnLogout(const std::string strValue)
    {
        m_strResponse = R"(* BYE LOGOUT received\r\n)";
        m_strResponse += R"(A09 OK LOGOUT Completed\r\n)";
        return true;
    }

    bool CImapServerProtoHandler::OnLogin(const std::string strValue)
    {
        m_strResponse = "A03 OK Success login ok\r\n";
        return true;
    }
}