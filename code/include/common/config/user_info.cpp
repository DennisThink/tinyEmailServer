#include "user_info.h"
#include "ProtoUtil.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <istream>
namespace tiny_email 
{

    bool ParseEmailForSmtp(const std::string& strContext,email_info_t& email)
    {
        StringArray parseArray = CProtoUtil::SplitStringByLine(strContext);
        std::size_t index = 0;
        std::size_t Count = parseArray.size();
        for(index = 0 ; index < Count ; index++)
        {
            if(parseArray[index].find("From: ") != std::string::npos)
            {
                std::string strName;
                std::string strAddr;
                CProtoUtil::ParseFromToString(parseArray[index],strName,strAddr);
                email.emailSender_=senderReceiverInfo(strAddr);
            }
            
            if(parseArray[index].find("To: ") != std::string::npos)
            {
                std::string strName;
                std::string strAddr;
                CProtoUtil::ParseFromToString(parseArray[index],strName,strAddr);
                email.emailReceiver_.name_ = CProtoUtil::Trim(strName);
                email.emailReceiver_.emailAddr_ = CProtoUtil::Trim(strAddr);
            }

            if(parseArray[index].find("Subject:") != std::string::npos)
            {
                std::size_t startPos = std::string("Subject:").length();
                email.subject_ = CProtoUtil::Trim(parseArray[index].substr(startPos,parseArray[index].length()-startPos));
            }

            if(parseArray[index] == "\r\n")
            {
                while(index < Count)
                {
                    email.context_.append(parseArray[index]);
                    index++;
                }
            }
        }
        return true;
    }

    bool Pop3RspFromEmail(const email_info_t&email,std::string& strRsp)
    {
        std::string strDate="Date: "+CProtoUtil::TimeToString(email.emailTime_)+"\r\n";
        std::string strFrom = "From: "+email.emailSender_.emailAddr_+" \r\n";
        std::string strTo = "To: "+email.emailReceiver_.emailAddr_+" \r\n";
        std::string strSubject = "Subject: "+email.subject_ +"  \r\n";
        std::string strContent = "\r\n"+email.context_+" \r\n.\r\n";
        strRsp = strDate+strFrom+strTo+strSubject+strContent;
        return true;
    }
    bool ParseEmailFromRecv(const std::string& strContext,email_info_t& email)
    {
        StringArray parseArray = CProtoUtil::SplitStringByLine(strContext);
        std::size_t index = 0;
        std::size_t Count = parseArray.size();
        for(index = 0 ; index < Count ; index++)
        {
            if(parseArray[index].find("Date: ") != std::string::npos)
            {
                email.date_=parseArray[index];
            }
            if(parseArray[index].find("From: ") != std::string::npos)
            {
                std::string strName;
                std::string strAddr;
                CProtoUtil::ParseFromToString(parseArray[index],strName,strAddr);
                email.emailSender_=senderReceiverInfo(strAddr);
            }
            
            if(parseArray[index].find("To: ") != std::string::npos)
            {
                std::string strName;
                std::string strAddr;
                CProtoUtil::ParseFromToString(parseArray[index],strName,strAddr);
                email.copyReceivers_.push_back(senderReceiverInfo(strAddr));
            }

            if(parseArray[index].find("Content-Type: ") != std::string::npos)
            {
                while(index < Count)
                {
                    email.context_.append(parseArray[index]);
                    index++;
                }
            }
        }
        return true;
    }


    /*
    {
    "domainName":"email.test.com";
    "databaseName":"testemail.db",
    "smtpServer":{
    "ip":"127.0.0.1",
    "port":2125
    },
    "pop3Server":{
    "ip":"127.0.0.1",
    "port":2110
    },
    "imapServer":{
     "ip":"127.0.0.1",
    "port":2134
    }
    }
    */
    bool ParseConfigFromFile(const std::string strConfigName, email_server_config& config)
    {
        std::ifstream ifile(strConfigName);
        if (ifile.is_open())
        {
            nlohmann::json jData = nlohmann::json::parse(ifile);
            if (jData["domainName"].is_string())
            {
                config.m_strDomain = jData["domainName"].get<std::string>();
            }

            if (jData["databaseName"].is_string())
            {
                config.m_strDataBaseName = jData["databaseName"].get<std::string>();
            }

            if (jData["smtpServer"].is_object())
            {
                if (jData["smtpServer"]["ip"].is_string())
                {
                    config.m_smtpServer.strIp_ = jData["smtpServer"]["ip"].get<std::string>();
                }
                if (jData["smtpServer"]["port"].is_number_integer())
                {
                    config.m_smtpServer.strIp_ = jData["smtpServer"]["port"].get<int>();
                }
            }
            if (jData["pop3Server"].is_object())
            {
                if (jData["pop3Server"]["ip"].is_string())
                {
                    config.m_pop3Server.strIp_ = jData["pop3Server"]["ip"].get<std::string>();
                }
                if (jData["pop3Server"]["port"].is_number_integer())
                {
                    config.m_pop3Server.strIp_ = jData["pop3Server"]["port"].get<int>();
                }
            }
            if (jData["imapServer"].is_object())
            {
                if (jData["imapServer"]["ip"].is_string())
                {
                    config.m_imapServer.strIp_ = jData["imapServer"]["ip"].get<std::string>();
                }
                if (jData["imapServer"]["port"].is_number_integer())
                {
                    config.m_imapServer.strIp_ = jData["imapServer"]["port"].get<int>();
                }
            }
            return true;
        }
        else
        {
            return false;
        }
    }
}
