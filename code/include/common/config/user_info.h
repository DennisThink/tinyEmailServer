#ifndef _CPP_MAIL_USER_INFO_H_
#define _CPP_MAIL_USER_INFO_H_
#include <string>
#include <vector>
#include <algorithm>
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
    };

    struct ss_server_config
    {
        
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
    public:
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
    };
    bool ParseEmailForSmtp(const std::string& strContext,email_info_t& email);
    bool ParseEmailFromRecv(const std::string& strContext,email_info_t& email);
    using EmailInfoArray_t = std::vector<email_info_t>;
}
#endif