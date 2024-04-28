#include "CProtoCmd.h"
#include "ProtoUtil.h"
#include <algorithm>
namespace tiny_email
{
    CSmtpProtoCmd::CSmtpProtoCmd(ProtoCode_t code,std::string strMsg,bool bFinish)
    {
        m_code = code;
        m_message = strMsg;
        m_bFinish = bFinish;
    }

    CSmtpProtoCmd CSmtpProtoCmd::FromString(const std::string& strValue)
    {
        int code = 0;
        std::string strMessage ="";
        bool bFinish = false;
        if(CProtoUtil::SplitLine(strValue,code,strMessage,bFinish))
        {
            return CSmtpProtoCmd(static_cast<ProtoCode_t>(code),strMessage,bFinish);
        }
        else
        {
            return CSmtpProtoCmd(ProtoCode_t::CODE_BAD_CMD_1,"Bad Cmd",bFinish);
        }
    }


    CSmtpProtoCmd CSmtpProtoCmd::FromClientString(const std::string& strValue)
    {
        int code = 0;
        std::string strMessage ="";
        bool bFinish = false;
        if(CProtoUtil::SplitLine(strValue,code,strMessage,bFinish))
        {
            return CSmtpProtoCmd(static_cast<ProtoCode_t>(code),strMessage,bFinish);
        }
        else
        {
            auto pos = strValue.find("HELO ");
            if(std::string::npos != pos)
            {
                std::string strMsg = strValue.substr(pos+1,strValue.length()-pos-1);
                return CSmtpProtoCmd(ProtoCode_t::CODE_HELO,strMsg,false);
            }
            return CSmtpProtoCmd(ProtoCode_t::CODE_BAD_CMD_1,"Bad Cmd",bFinish);
        }
    }

    CSmtpProtoCmd CSmtpProtoCmd::CreateWelComeCmd()
    {
        std::string strMsg=" cppemail.com Anti-spam GT for Coremail System";
        return CSmtpProtoCmd(ProtoCode_t::CODE_220,strMsg,false);
    }

    CSmtpProtoCmd CSmtpProtoCmd::Create250Cmd()
    {
        std::string strMsg=" 8BITMIME";
        return CSmtpProtoCmd(ProtoCode_t::CODE_250,strMsg,false);
    }






    std::string& trim(std::string &s) 
    {
        if (s.empty()) 
        {
            return s;
        }
        s.erase(0,s.find_first_not_of(" "));
        s.erase(s.find_last_not_of(" ") + 1);
        return s;
    }

    PARSE_SMTP_RESULT CSmtpProtoReqCmd::FromString(const std::string& strValue,CSmtpProtoReqCmd& cmd)
    {
        auto posSpace = strValue.find(" ");
        if(posSpace != std::string::npos)
        {
            std::string strPrefix = strValue.substr(0,posSpace);
            std::transform(strPrefix.begin(),strPrefix.end(),strPrefix.begin(),::toupper);
            std::string strMessage = strValue.substr(posSpace,strValue.length()-posSpace);
            strMessage = trim(strMessage);
            /**
             * @brief const std::string strEHLO="EHLO smtp.cppemail.com\r\n";
                const std::string strAUTH_LOGIN="AUTH LOGIN\r\n";
                const std::string strSendUserName="ZGVubmlzQGNwcGVtYWlsLmNvbQ==\r\n";
                const std::string strSendPassword="ZGVubmlz\r\n";
                const std::string strSendMailFrom="MAIL FROM:dennis@cppemail.com\r\n";
                const std::string strSendMailTo="RCPT TO:test@cppemail.com\r\n";
                const std::string strSendData="DATA\r\n";
                const std::string strSendEmailBody="This is a Test Email\r\n";
                const std::string strSendEmailEnd=".\r\n";
                const std::string strSendQuit="QUIT\r\n";
             * 
             */
            if(strPrefix=="EHLO"||strPrefix=="HELO")
            {
                cmd = CSmtpProtoReqCmd(SMTP_CMD_t::SMTP_CMD_HELO_EHLO,strMessage);
                return PARSE_SMTP_RESULT::PARSE_SMTP_SUCCEED;
            }
            else if(strPrefix=="AUTH")
            {
                cmd = CSmtpProtoReqCmd(SMTP_CMD_t::SMTP_CMD_AUTH,strMessage);
                return PARSE_SMTP_RESULT::PARSE_SMTP_SUCCEED;
            }
            else if(strPrefix =="MAIL")
            {
                cmd = CSmtpProtoReqCmd(SMTP_CMD_t::SMTP_CMD_MAIL_FROM,strMessage);
                return PARSE_SMTP_RESULT::PARSE_SMTP_SUCCEED;
            }
            else if(strPrefix =="RCPT")
            {
                cmd = CSmtpProtoReqCmd(SMTP_CMD_t::SMTP_CMD_RECP_TO,strMessage);
                return PARSE_SMTP_RESULT::PARSE_SMTP_SUCCEED;
            }
            
        }
        std::string strPrefix = strValue;
        std::transform(strPrefix.begin(),strPrefix.end(),strPrefix.begin(),::toupper);
        if(strPrefix=="QUIT\r\n")
        {
            cmd = CSmtpProtoReqCmd(SMTP_CMD_t::SMTP_CMD_QUIT,"");
            return PARSE_SMTP_RESULT::PARSE_SMTP_SUCCEED;
        }
        else if(strPrefix =="DATA\r\n")
        {
            cmd = CSmtpProtoReqCmd(SMTP_CMD_t::SMTP_CMD_DATA,"");
            return PARSE_SMTP_RESULT::PARSE_SMTP_SUCCEED;
        }
        else
        {
            return PARSE_SMTP_RESULT::PARSE_SMTP_SYNTAX_ERROR;
        }
    }

}