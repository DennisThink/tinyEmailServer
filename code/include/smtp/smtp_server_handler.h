#pragma once
#ifndef _CPP_EMAIL_SERVER_SMTP_HANDLER_H_
#define _CPP_EMAIL_SERVER_SMTP_HANDLER_H_
#include "CProtoCode.h"
#include <string>
namespace tiny_email
{
    class CSmtpServerHandler
    {
    public:
        CSmtpServerHandler();
        bool OnClientReq(const std::string strValue);
        std::string GetResponse();
        std::string GetUserName();
        std::string GetPassowrd();
        virtual ~CSmtpServerHandler();
    private:
        std::string GetNextStepCmd(const Smtp_Server_Step_t& step);
        Smtp_Server_Step_t m_step;
        bool m_bAuthFinish;
        std::string m_strEmailDomain;
        std::string m_emailData;
        int m_errorCmdCount;
        std::string m_strResponse;
    public:
        bool OnEhloReq(const std::string strReq);
        bool OnAuthLoginReq(const std::string strReq);
        bool OnPasswordReq(const std::string strReq);
        bool OnNamePassVerifyReq(const std::string strReq);
        bool OnMailFromReq(const std::string strReq);
        bool OnRcptToReq(const std::string strReq);
        bool OnDataReq(const std::string strReq);
        bool OnData(const std::string strReq);
    };
}
#endif