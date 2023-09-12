#include "SqliteDataBase.h"
#include <iostream>
int main(int argc,char * argv[])
{
    tiny_email::CSqliteDataBase db("email.db");
    if(db.IsUserExist("test"))
    {
        std::cout<<"test is Exist"<<std::endl;
    }
    else
    {
        std::cerr<<"test is not exist"<<std::endl;
    }
    tiny_email::email_info_t info;
    info.emailSender_.name_ = "test";
    info.emailReceiver_.name_ = "test1";
    info.subject_ = "Demo Test";
    info.context_ = "test";
    db.SaveSendMailInfo(info);
    std::cout<<"database_demo"<<std::endl;
    return 0;
}