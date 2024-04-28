#ifndef _CPP_EMAIL_PROTO_CMD_H_
#define _CPP_EMAIL_PROTO_CMD_H_
#include <string>
#include "CProtoCode.h"
namespace tiny_email
{
    class CSmtpProtoCmd
    {
    public:
        static CSmtpProtoCmd FromString(const std::string& strValue);
        static CSmtpProtoCmd FromClientString(const std::string& strValue);
        static CSmtpProtoCmd CreateWelComeCmd();
        static CSmtpProtoCmd Create250Cmd();
        ProtoCode_t GetCode() const 
        {
            return m_code;
        }

        std::string GetMessage() const 
        {
            return m_message;
        }
        
        std::string GetSendContext() const
        {
            return std::to_string(m_code)+""+m_message;
        }
        bool isFinish() const 
        {
            return m_bFinish;
        }
    private:

        explicit CSmtpProtoCmd(ProtoCode_t code,std::string message,bool bFinish);
        ProtoCode_t m_code;
        std::string m_message;
        bool m_bFinish=false;
    };

    class CSmtpProtoReqCmd
    {
    public:
        explicit CSmtpProtoReqCmd()
        {
            m_code = SMTP_CMD_t::SMTP_CMD_NO_CMD;
            m_message = "";
        }
        static PARSE_SMTP_RESULT FromString(const std::string& strValue,CSmtpProtoReqCmd& cmd);
        SMTP_CMD_t GetCode() const 
        {
            return m_code;
        }

        std::string GetMessage() const 
        {
            return m_message;
        }
    private:
        explicit CSmtpProtoReqCmd(SMTP_CMD_t code,std::string message)
        {
            m_code = code;
            m_message = message;
        }
        SMTP_CMD_t m_code;
        std::string m_message;
    };
}
#endif