#ifndef _TINY_EMAIL_CIMAP_PROTO_CMD_H_
#define _TINY_EMAIL_CIMAP_PROTO_CMD_H_
#include "CProtoCode.h"
#include <string>
namespace tiny_email
{
    class CImapProtoCmd
    {
    public:
        static CImapProtoCmd FromString(const std::string& strValue);
        ImapCode_t GetCode() const
        {
            return m_code;
        }

        std::string GetCmdId() const
        {
            return m_strCmdId;
        }

        std::string GetMessage() const
        {
            return m_message;
        }

        bool isFinish() const
        {
            return m_bFinish;
        }
    private:
        explicit CImapProtoCmd(const std::string strCmdId,ImapCode_t code, std::string message, bool bFinish)
        {
            m_strCmdId = strCmdId;
            m_code = code;
            m_message = message;
            m_bFinish = bFinish;
        }
        std::string m_strCmdId;
        ImapCode_t m_code;
        std::string m_message;
        bool m_bFinish = false;
    };

    class CImapProtoReqCmd
    {
    public:
        explicit CImapProtoReqCmd()
        {
            m_code = POP3_CMD_t::POP3_CMD_NO_CMD;
            m_message = "";
        }
        static PARSE_POP3_RESULT FromString(const std::string& strValue, CImapProtoReqCmd& cmd);
        POP3_CMD_t GetCode() const
        {
            return m_code;
        }

        std::string GetMsg() const
        {
            return m_message;
        }

    private:
        explicit CImapProtoReqCmd(POP3_CMD_t code, std::string message)
        {
            m_code = code;
            m_message = message;
        }
        POP3_CMD_t m_code;
        std::string m_message;
    };
}
#endif