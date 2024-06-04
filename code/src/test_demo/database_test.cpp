#include "doctest.h"
#include "SqliteDataBase.h"
#include "ProtoUtil.h"
#include <iostream>
TEST_CASE("database_test") {
    tiny_email::CSqliteDataBase db("email_test.db");
    CHECK_FALSE(db.IsUserExist("test@test.com"));
    db.AddUser("test@test.com","test");
    CHECK(db.IsUserExist("test@test.com"));
    CHECK(db.IsPasswordRight("test@test.com","test"));

    tiny_email::email_info_t info;
    {
        info.emailTime_ = tiny_email::CProtoUtil::Now();
        info.emailSender_.name_ = "test";
        info.emailSender_.emailAddr_ = "test@test.com";
        info.emailReceiver_.name_ = "test1";
        info.emailReceiver_.emailAddr_ = "test1@test.com";
        info.subject_ = "Demo Test";
        info.context_ = "test";
        CHECK(db.SaveSendMailInfo(info));
    }
    {
        info.emailTime_ = tiny_email::CProtoUtil::Now();
        info.emailSender_.name_ = "test";
        info.emailSender_.emailAddr_ = "test@test.com";
        info.emailReceiver_.name_ = "test1";
        info.emailReceiver_.emailAddr_ = "test1@test.com";
        info.subject_ = "Demo Test";
        info.context_ = "test";
        CHECK(db.SaveSendMailInfo(info));
    }
    tiny_email::EmailInfoArray_t emailArray;
    CHECK(db.GetRecvMailInfo("test1@test.com",emailArray));
    {
        CHECK_FALSE(emailArray.empty());
        if (!emailArray.empty())
        {
            CHECK_EQ((emailArray[0].emailSender_.emailAddr_), "test@test.com");
            CHECK_EQ((emailArray[0].emailReceiver_.emailAddr_), "test1@test.com");
            CHECK_EQ((emailArray[0].subject_), "Demo Test");
            CHECK_EQ((emailArray[0].context_), "test");
            for (auto item : emailArray)
            {
                std::cout << item.emailTime_ << std::endl;
            }
        }
        else
        {
            CHECK_EQ(1, 2);
        }
    }
    CHECK(db.RemoveUser("test@test.com"));
    CHECK_FALSE(db.IsUserExist("test@test.com"));
}