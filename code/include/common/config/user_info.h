#ifndef _CPP_MAIL_USER_INFO_H_
#define _CPP_MAIL_USER_INFO_H_
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
namespace tiny_email
{
    using StringArray = std::vector<std::string>; 
    struct user_info
    {
        std::string userName_;
        std::string userPassword_;  
    };


    struct server_config
    {
        std::string strIp_;
        int         port_;
        std::string toString()
        {
            std::string strResult=" ";
            strResult += strIp_;
            strResult += ":";
            strResult += std::to_string(port_);
            strResult += " ";
            return strResult;

        }
    };

    struct ss_server_config
    {
        
    };

    struct email_server_config
    {
        std::string m_strDomain;
        std::string m_strDataBaseName;
        server_config m_smtpServer;
        server_config m_pop3Server;
        server_config m_imapServer;

        std::string toString()
        {
            std::string strResult = " ";
            strResult += " Domain: ";
            strResult += m_strDomain;
            strResult += "\r\n";

            strResult += " DataBase: ";
            strResult += m_strDataBaseName;
            strResult += "\r\n";

            strResult += " SMTP: ";
            strResult += m_smtpServer.toString();
            strResult += "\r\n";

            strResult += " POP3: ";
            strResult += m_pop3Server.toString();
            strResult += "\r\n";


            strResult += " IMAP: ";
            strResult += m_imapServer.toString();
            strResult += "\r\n";

            return strResult;
        }
    };
    struct senderReceiverInfo
    {
        std::string name_;
        std::string emailAddr_;
        senderReceiverInfo()
        {
            
        }
        senderReceiverInfo(std::string emailAddr)
        {
            name_=emailAddr;
            emailAddr_=emailAddr;
        }
    };

    using SenderReceiverInfoArray = std::vector<senderReceiverInfo>;
    struct email_info_t
    {
        std::string date_;
        senderReceiverInfo emailSender_;
        senderReceiverInfo emailReceiver_;
        SenderReceiverInfoArray copyReceivers_;
        std::string subject_;
        SenderReceiverInfoArray secretReceivers_;
        std::string context_;
        std::size_t emailBytes_;
        std::time_t emailTime_;
    public:
        email_info_t()
        {
            emailBytes_ = 0;
            emailTime_ = time(nullptr);
        }
        bool IsReceiver(const std::string strUser) const
        {
            if(!copyReceivers_.empty())
            {
                for(const auto& item:copyReceivers_)
                {
                    if(item.emailAddr_ == strUser)
                    {
                        return true;
                    }
                }
            }

            if(!secretReceivers_.empty())
            {
                for(const auto& item:secretReceivers_)
                {
                    if(item.emailAddr_ == strUser)
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        std::string toString() const{
            std::string strRsp =  "FROM: "+this->emailSender_.emailAddr_+"\r\n";
            strRsp += "TO: "+this->emailReceiver_.emailAddr_+"\r\n";
            strRsp += "Subject: "+this->subject_+"\r\n";
            strRsp += "Context: "+this->context_+"\r\n";
            return strRsp;
        }
    };
    bool ParseConfigFromFile(const std::string strConfigName, email_server_config& config);
    bool ParseEmailForSmtp(const std::string& strContext,email_info_t& email);
    bool ParseEmailFromRecv(const std::string& strContext,email_info_t& email);
    bool Pop3RspFromEmail(const email_info_t&email,std::string& strRsp);
    using EmailInfoArray_t = std::vector<email_info_t>;
}
#endif