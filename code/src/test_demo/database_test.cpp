#include "doctest.h"
#include "SqliteDataBase.h"
#include <iostream>
TEST_CASE("database_test") {
    tiny_email::CSqliteDataBase db("email_test.db");
    CHECK_FALSE(db.IsUserExist("test"));
    db.AddUser("test","test");
    CHECK(db.IsUserExist("test"));

    tiny_email::email_info_t info;
    info.emailSender_.name_ = "test";
    info.emailReceiver_.name_ = "test1";
    info.subject_ = "Demo Test";
    info.context_ = "test";
    CHECK(db.SaveSendMailInfo(info));
    tiny_email::EmailInfoArray_t emailArray;
    CHECK(db.GetRecvMailInfo("test1",emailArray));
    CHECK(db.RemoveUser("test"));
    CHECK_FALSE(db.IsUserExist("test"));
}