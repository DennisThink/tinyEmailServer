#pragma once
#ifndef _CPP_EMAIL_SERVER_SMTP_HANDLER_H_
#define _CPP_EMAIL_SERVER_SMTP_HANDLER_H_
#include "CProtoCode.h"
#include "DataBaseInterface.h"
#include <string>
#include "CEmailServerProtoHandler.h"
namespace tiny_email
{
    class CSmtpServerProtoHandler:public CEmailServerProtoInterface
    {
    public:
        CSmtpServerProtoHandler(CDataBaseInterface_SHARED_PTR dbPtr,const std::string strDomainName);
        virtual bool OnClientReq(const std::string strValue) override;
        virtual bool IsFinished() override;
        virtual std::string GetResponse() override;
        virtual ~CSmtpServerProtoHandler();
    private:
        std::string GetNextStepCmd(const Smtp_Server_Step_t& step);
        Smtp_Server_Step_t m_step;
        std::string m_strUserName;
        std::string m_strUserAddr;
        std::string m_strPassword;
        bool m_bAuthFinish;
        std::string m_emailData;
        int m_errorCmdCount;
        std::string m_strResponse;
        bool m_bFinished;
    public:
        bool OnEhloReq(const std::string strReq);
        bool OnAuthLoginReq(const std::string strReq);
        bool OnAuthPlainReq(const std::string strReq);
        bool OnPasswordReq(const std::string strReq);
        bool OnNamePassVerifyReq(const std::string strReq);
        bool OnMailFromReq(const std::string strReq);
        bool OnRcptToReq(const std::string strReq);
        bool OnDataReq(const std::string strReq);
        bool OnData(const std::string strReq);
    };
}
#endif