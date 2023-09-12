#ifndef _CPP_EMAIL_DATA_BASE_INTERFACE_H_
#define _CPP_EMAIL_DATA_BASE_INTERFACE_H_
#include <string>
#include <memory>
#include "user_info.h"
namespace tiny_email 
{
    class CDataBaseInterface
    {
    public:
        virtual ~CDataBaseInterface()=default;
        virtual bool IsUserExist(std::string strUserName)=0;
        virtual bool IsPasswordRight(std::string strUserName,std::string strPasswd)=0;
        virtual bool SaveSendMailInfo(const email_info_t& email)=0;
        //call multi to get all
        virtual bool GetRecvMailInfo(const std::string userName,EmailInfoArray_t& email)=0;
    };
    using CDataBaseInterface_SHARED_PTR = std::shared_ptr<CDataBaseInterface>;
}
#endif