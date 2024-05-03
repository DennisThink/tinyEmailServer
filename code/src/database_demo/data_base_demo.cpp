#include "SqliteDataBase.h"
#include <iostream>
int main(int argc,char * argv[])
{
    std::cout << argc << "   " << argv[0] << std::endl;
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
    tiny_email::EmailInfoArray_t emailArray;
    if(db.GetRecvMailInfo("test1",emailArray))
    {
        for(auto item:emailArray)
        {
            std::cout<<"Sender: "<<item.emailSender_.name_<<std::endl;
            std::cout<<"Receiver: "<<item.emailReceiver_.name_<<std::endl;
            std::cout<<"Subject: "<<item.subject_<<std::endl;
            std::cout<<"Content: "<<item.context_<<std::endl;
        }
    }
    std::cout<<"database_demo"<<std::endl;
    return 0;
}