#pragma once
#ifndef _CPP_EMAIL_SERVER_POP3_HANDLER_H_
#define _CPP_EMAIL_SERVER_POP3_HANDLER_H_
#include "CProtoCode.h"
#include "DataBaseInterface.h"
#include <string>
#include "Log.h"
#include "CEmailServerProtoHandler.h"
namespace tiny_email
{
    class CPop3ServerProtoHandler: public CEmailServerProtoInterface
    {
    public:
        CPop3ServerProtoHandler(CDataBaseInterface_SHARED_PTR dbPtr,const std::string strDomainName);
        virtual bool OnClientReq(const std::string strValue) override;
        virtual std::string GetResponse() override;
        virtual bool IsFinished() override;
        virtual ~CPop3ServerProtoHandler();
    protected:
        bool OnUser(const std::string& strUser);
        bool OnPassword(const std::string& strPasswd);
        bool OnNoOp(const std::string& strRecv);
        bool OnState(const std::string& strRecv);
        bool OnRetr(const std::string& strRecv);
        bool OnList(const std::string& strRecv);
        bool OnCapa(const std::string& strRecv);
        bool OnQuit(const std::string& strRecv);
    private:
        void MailInfoUpdate();
        std::string GetNextStepCmd(const POP3_SERVER_STEP_t& step);
        bool OnRecv(const std::string &strRecv);
        std::string GetPassWordOkSend();

        POP3_SERVER_STEP_t m_step;
        std::string m_strUserAddr;
        std::string m_strUserName;
        std::string m_strPassword;
        bool m_bAuthFinish;
        std::string m_emailData;
        int m_errorCmdCount;
        std::string m_strResponse;
        std::string m_strEmailStateSummary;//Total 
        std::string m_strEmailListDetail;//Each email a line
        EmailInfoArray_t m_emailArray;
        bool m_bFinished;
    public:
    };
}
#endif