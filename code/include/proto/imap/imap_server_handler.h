#ifndef _TINY_EMAIL_IMAP_SERVER_HANDLER_H_
#define _TINY_EMAIL_IMAP_SERVER_HANDLER_H_
#include "CProtoCode.h"
#include "DataBaseInterface.h"
#include <string>
#include "CEmailServerProtoHandler.h"
namespace tiny_email
{
    class CImapServerProtoHandler: public CEmailServerProtoInterface
    {
    public:
        CImapServerProtoHandler(CDataBaseInterface_SHARED_PTR dbPtr, const std::string strDomainName);
        virtual bool OnClientReq(const std::string strValue) override;
        virtual std::string GetResponse() override;
        virtual bool IsFinished() override;
        virtual ~CImapServerProtoHandler();
    public:
        bool OnClientConnect(const std::string strValue);
        bool OnCapability(const std::string strValue);
        bool OnNoop(const std::string strValue);
        bool OnSelect(const std::string strValue);
        bool OnExamine(const std::string strValue);
        bool OnSearchAll(const std::string strValue);
        bool OnFetchOne(const std::string strValue);
        bool OnList(const std::string strValue);
        bool OnLogout(const std::string strValue);
        bool OnLogin(const std::string strValue);
    private:
        bool IsCommandComplete(const std::string strValue);
        void MailInfoUpdate();
        std::string GetNextStepCmd(const POP3_SERVER_STEP_t& step);
        bool OnRecv(const std::string& strRecv);
        std::string GetPassWordOkSend();

        IMAP_SERVER_STEP_t m_step;
        std::string m_strUserAddr;
        std::string m_strUserName;
        std::string m_strPassword;
        bool m_bAuthFinish;
        std::string m_emailData;
        int m_errorCmdCount;
        std::string m_strReceive;
        std::string m_strResponse;
        std::string m_strEmailStateSummary;//Total 
        std::string m_strEmailListDetail;//Each email a line
        EmailInfoArray_t m_emailArray;
    public:
    };
}
#endif