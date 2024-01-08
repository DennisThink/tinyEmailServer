#include "CMemDataBase.h"
#include <iostream>
namespace tiny_email
{
    CMemDataBase::CMemDataBase()
    {

    }

    std::string GetUserNameFromEmailAddr(std::string strEmailAddr)
    {
        auto atPos = strEmailAddr.find("@");
        if(atPos != std::string::npos)
        {
            return strEmailAddr.substr(0,atPos);
        }
        return strEmailAddr;
    }
    bool CMemDataBase::IsUserExist(std::string strUserName)
    {
        std::string strUser = GetUserNameFromEmailAddr(strUserName);
        if(strUser == "sender" || strUser == "receiver")
        {
            return true;
        }
        return false;
    }

    bool CMemDataBase::IsPasswordRight(std::string strUserName,std::string strPassword)
    {
        if(IsUserExist(strUserName))
        {
            std::string strUser = GetUserNameFromEmailAddr(strUserName);
            if(strPassword == strUser)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        return false;
    }


    bool CMemDataBase::SaveSendMailInfo(const email_info_t& email)
    {
        m_sendMails.emplace_back(email);
        return true;
    }    
    
    //call multi to get all
    bool CMemDataBase::GetRecvMailInfo(const std::string userName,EmailInfoArray_t& email)
    {
        std::cout<<userName<<std::endl;
        EmailInfoArray_t lastEmails;
        for(const auto& item:m_sendMails)
        {
            if(item.IsReceiver(userName))
            {
                email.push_back(item);
            }
            else
            {
                lastEmails.push_back(item);
            }
        }
        m_sendMails = lastEmails;
        return true;
    }


    bool CMemDataBase::AddUser(std::string strUserName,std::string strPassword)
    {
        return false;
    }
    bool CMemDataBase::RemoveUser(std::string strUserName)
    {
        return false;
    }
}