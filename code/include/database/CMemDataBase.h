#ifndef _CPP_EMAIL_MEM_DATABASE_H_
#define _CPP_EMAIL_MEM_DATABASE_H_
#include "DataBaseInterface.h"
namespace tiny_email
{
    class CMemDataBase:public CDataBaseInterface
    {
    public:
        virtual bool IsUserExist(std::string strUserName)override final;
        virtual bool IsPasswordRight(std::string strUserName,std::string strPassword)override final;
        virtual bool SaveSendMailInfo(const email_info_t& email) override final;
        //call multi to get all
        virtual bool GetRecvMailInfo(const std::string userName,EmailInfoArray_t& email) override final;
        explicit CMemDataBase();
        virtual ~CMemDataBase()=default;
    private:
        EmailInfoArray_t m_sendMails;
        EmailInfoArray_t m_recvMails;
        StringArray m_userList;
    };
    using CMemDataBase_SHARED_PTR = std::shared_ptr<CMemDataBase>;
}
#endif