#include "SqliteDataBase.h"
#include "SQLiteCpp/SQLiteCpp.h"
#include "ProtoUtil.h"
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
            std::cout << "Exception:  " << ec.what() << std::endl;
            if (!g_db)
            {
                try
                {
                    g_db = std::make_shared<SQLite::Database>(strDbFileName, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
                    std::string strCreateUserTable("CREATE TABLE T_USER(ID INTEGER PRIMARY KEY AUTOINCREMENT,USER_NAME TEXT,PASS_WORD TEXT);");
                    SQLite::Transaction tr(*g_db);
                    g_db->exec(strCreateUserTable);
                    std::string strCreateSendEmailTable("CREATE TABLE T_EMAIL_SEND(ID INTEGER PRIMARY KEY AUTOINCREMENT,SENDER_NAME TEXT,SENDER_EMAIL TEXT,RECEIVER_NAME TEXT,RECEIVER_EMAIL TEXT,SUBJECT TEXT,CONTENT TEXT,TIME TEXT);");
                    g_db->exec(strCreateSendEmailTable);
                    tr.commit();
                }
                catch (std::exception &ec2)
                {
                    std::cout << "Exception:  " << ec2.what() << std::endl;
                }
            }
        }

        InitUserArrayFromDB();
    }

    /*void CSqliteDataBase::InitDemoData()
    {
        SQLite::Transaction tr(*g_db);
        g_db->exec("INSERT INTO T_USER(USER_NAME,PASS_WORD) VALUES(\"test1\", \"test1_pass\");");
        g_db->exec("INSERT INTO T_USER(USER_NAME,PASS_WORD) VALUES(\"test2\", \"test2_pass\");");
        tr.commit();
    }*/
    void CSqliteDataBase::InitUserArrayFromDB()
    {
        m_userArray.clear();
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
                std::cout << "Exception:  " << ec.what() << std::endl;
            }
        }
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
        for (auto item : m_userArray)
        {
            if (strUserName == item.userName_ && strPassword == item.userPassword_)
            {
                return true;
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
                std::string strInsertEmailSend = "INSERT INTO T_EMAIL_SEND(SENDER_NAME,SENDER_EMAIL,RECEIVER_NAME,RECEIVER_EMAIL,SUBJECT,CONTENT,TIME) VALUES(?,?,?,?,?,?,?)";
                SQLite::Statement query(*g_db, strInsertEmailSend);
                query.bind(1, email.emailSender_.name_);
                query.bind(2, email.emailSender_.emailAddr_);
                query.bind(3,email.emailReceiver_.name_);
                query.bind(4, email.emailReceiver_.emailAddr_);
                query.bind(5, email.subject_);
                query.bind(6, email.context_);
                query.bind(7, std::to_string(email.emailTime_));
                query.exec();
                bResult = true;
            }
            catch (std::exception &ec)
            {
                std::cout << "Exception:  " << ec.what() << std::endl;
                bResult = false;
            }
        }
        return bResult;
    }

    // call multi to get all
    bool CSqliteDataBase::GetRecvMailInfo(const std::string userName, EmailInfoArray_t &emailArray)
    {
        if (g_db)
        {
            try
            {
                std::string strQueryEmailReceive = "SELECT SENDER_NAME,SENDER_EMAIL,RECEIVER_NAME,RECEIVER_EMAIL,SUBJECT,CONTENT FROM T_EMAIL_SEND WHERE RECEIVER_EMAIL == ?";
                SQLite::Statement query(*g_db, strQueryEmailReceive);
                query.bind(1, userName);
                while (query.executeStep())
                {
                    email_info_t email;
                    email.emailSender_.name_ = query.getColumn(0).getString();
                    email.emailSender_.emailAddr_ = query.getColumn(1).getString();
                    email.emailReceiver_.name_ = query.getColumn(2).getString();
                    email.emailReceiver_.emailAddr_ = query.getColumn(3).getString();
                    email.subject_ = query.getColumn(4).getString();
                    email.context_ = query.getColumn(5).getString();
                    emailArray.push_back(email);
                }
                return true;
            }
            catch (std::exception &ec)
            {
                std::cout << "Exception:  " << ec.what() << std::endl;
            }
        }
        return false;
    }

    bool CSqliteDataBase::AddUser(std::string strUserName, std::string strPassword)
    {
        bool bResult = false;
        if (g_db)
        {
            try
            {
                std::string strInsertUser = "INSERT INTO T_USER(USER_NAME,PASS_WORD) VALUES(?,?)";
                SQLite::Statement query(*g_db, strInsertUser);
                query.bind(1, strUserName);
                query.bind(2, strPassword);
                query.exec();
                bResult = true;
            }
            catch (std::exception &ec)
            {
                std::cout << "Exception:  " << ec.what() << std::endl;
                bResult = false;
            }
        }
        InitUserArrayFromDB();
        return bResult;
    }
    bool CSqliteDataBase::RemoveUser(std::string strUserName)
    {
        bool bResult = false;
        if (g_db)
        {
            try
            {
                std::string strDeleteUser = "DELETE FROM T_USER WHERE USER_NAME=?";
                SQLite::Statement query(*g_db, strDeleteUser);
                query.bind(1, strUserName);
                query.exec();
                bResult = true;
            }
            catch (std::exception &ec)
            {
                std::cout << "Exception:  " << ec.what() << std::endl;
                bResult = false;
            }
        }
        InitUserArrayFromDB();
        return bResult;
    }
}