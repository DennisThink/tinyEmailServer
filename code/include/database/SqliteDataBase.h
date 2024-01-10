#ifndef _CPP_EMAIL_SQLITE_DATABASE_H_
#define _CPP_EMAIL_SQLITE_DATABASE_H_
#include "DataBaseInterface.h"
namespace tiny_email
{
    class CSqliteDataBase:public CDataBaseInterface
    {
    public:
        virtual bool AddUser(std::string strUserName,std::string strPassword) override final;
        virtual bool RemoveUser(std::string strUserName) override final;
        virtual bool IsUserExist(std::string strUserName)override final;
        virtual bool IsPasswordRight(std::string strUserName,std::string strPassword)override final;
        virtual bool SaveSendMailInfo(const email_info_t& email) override final;
        //call multi to get all
        virtual bool GetRecvMailInfo(const std::string userName,EmailInfoArray_t& email) override final;
        explicit CSqliteDataBase(const std::string strDbFileName);
        virtual ~CSqliteDataBase()=default;
    private:
        void InitUserArrayFromDB();
        //void InitDemoData();
        std::vector<user_info> m_userArray;
        EmailInfoArray_t m_sendMails;
        EmailInfoArray_t m_recvMails;
        StringArray m_userList;
    };
    using CSqliteDataBase_SHARED_PTR = std::shared_ptr<CSqliteDataBase>;
}
#endif