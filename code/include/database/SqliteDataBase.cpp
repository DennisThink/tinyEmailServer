#include "SqliteDataBase.h"
#include "SQLiteCpp/SQLiteCpp.h"
#include <iostream>
namespace tiny_email
{
    static std::shared_ptr<SQLite::Database> g_db = nullptr;
    CSqliteDataBase::CSqliteDataBase(const std::string strDbFileName)
    {
        try
        {
            g_db = std::make_shared<SQLite::Database>(strDbFileName, SQLite::OPEN_READWRITE);
        }
        catch (std::exception &ec)
        {
            if (!g_db)
            {
                try
                {
                    g_db = std::make_shared<SQLite::Database>(strDbFileName, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
                    std::string strCreateUserTable("CREATE TABLE T_USER(ID INTEGER PRIMARY KEY AUTOINCREMENT,USER_NAME TEXT,PASS_WORD TEXT);");
                    SQLite::Transaction tr(*g_db);
                    g_db->exec(strCreateUserTable);
                    g_db->exec("INSERT INTO T_USER(USER_NAME,PASS_WORD) VALUES(\"test\", \"test_pass\");");
                    std::string strCreateSendEmailTable("CREATE TABLE T_EMAIL_SEND(ID INTEGER PRIMARY KEY AUTOINCREMENT,SENDER TEXT,RECEIVER TEXT,SUBJECT TEXT,CONTENT TEXT);");
                    g_db->exec(strCreateSendEmailTable);
                    tr.commit();
                }
                catch (std::exception &ec2)
                {
                    std::cout<<"Exception:  "<<ec2.what()<<std::endl;
                }
            }
        }
        InitUserArrayFromDB();
    }

    void CSqliteDataBase::InitUserArrayFromDB()
    {
        if (g_db)
        {
            try
            {
                std::string strQueryNameAndPassword = "SELECT USER_NAME,PASS_WORD FROM T_USER";
                SQLite::Statement query(*g_db, strQueryNameAndPassword);
                while (query.executeStep())
                {
                    std::cout << "Name: " << query.getColumn(0) << "  Pass: " << query.getColumn(1) << std::endl;
                    user_info info{query.getColumn(0), query.getColumn(1)};
                    m_userArray.push_back(info);
                }
            }
            catch (std::exception &ec)
            {
            }
        }
    }

    std::string GetUserNameFromEmailAddr(std::string strEmailAddr)
    {
        auto atPos = strEmailAddr.find("@");
        if (atPos != std::string::npos)
        {
            return strEmailAddr.substr(0, atPos);
        }
        return strEmailAddr;
    }
    bool CSqliteDataBase::IsUserExist(std::string strUserName)
    {
        for (auto &item : m_userArray)
        {
            if (item.userName_ == strUserName)
            {
                return true;
            }
        }
        return false;
    }

    bool CSqliteDataBase::IsPasswordRight(std::string strUserName, std::string strPassword)
    {
        if (IsUserExist(strUserName))
        {
            std::string strUser = GetUserNameFromEmailAddr(strUserName);
            if (strPassword == strUser)
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

    bool CSqliteDataBase::SaveSendMailInfo(const email_info_t &email)
    {
        bool bResult = false;
        if (g_db)
        {
            try
            {
                std::string strInsertEmailSend = "INSERT INTO T_EMAIL_SEND(SENDER,RECEIVER,SUBJECT,CONTENT) VALUES(?,?,?,?)";
                SQLite::Statement query(*g_db, strInsertEmailSend);
                query.bind(1,email.emailSender_.name_);
                query.bind(2,email.emailReceiver_.name_);
                query.bind(3,email.subject_);
                query.bind(4,email.context_);
                query.exec();
                bResult = true;
            }
            catch (std::exception &ec)
            {
                bResult = false;
            }
        }
        return bResult;
    }

    // call multi to get all
    bool CSqliteDataBase::GetRecvMailInfo(const std::string userName, EmailInfoArray_t &email)
    {
        std::cout << userName << std::endl;
        EmailInfoArray_t lastEmails;
        for (const auto &item : m_sendMails)
        {
            if (item.IsReceiver(userName))
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
}