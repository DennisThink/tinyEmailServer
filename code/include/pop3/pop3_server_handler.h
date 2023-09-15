#pragma once
#ifndef _CPP_EMAIL_SERVER_POP3_HANDLER_H_
#define _CPP_EMAIL_SERVER_POP3_HANDLER_H_
#include "CProtoCode.h"
#include "DataBaseInterface.h"
#include <string>
namespace tiny_email
{
    class CPop3ServerHandler
    {
    public:
        CPop3ServerHandler(CDataBaseInterface_SHARED_PTR dbPtr);
        bool OnClientReq(const std::string strValue);
        std::string GetResponse();
        virtual ~CPop3ServerHandler();
    protected:
        bool OnUser(const std::string& strUser);
        bool OnPassword(const std::string& strPasswd);
        bool OnNoOp(const std::string& strRecv);
        bool OnState(const std::string& strRecv);
        bool OnRetr(const std::string& strRecv);
        bool OnList(const std::string& strRecv);
    private:
        std::string GetNextStepCmd(const POP3_SERVER_STEP_t& step);
        bool OnRecv(const std::string &strRecv);
        std::string GetPassWordOkSend();

        POP3_SERVER_STEP_t m_step;
        std::string m_strUserName;
        std::string m_strPassword;
        bool m_bAuthFinish;
        std::string m_strEmailDomain;
        std::string m_emailData;
        int m_errorCmdCount;
        std::string m_strResponse;
        CDataBaseInterface_SHARED_PTR m_db;
        EmailInfoArray_t m_emailArray;
    public:
    };
}
#endif