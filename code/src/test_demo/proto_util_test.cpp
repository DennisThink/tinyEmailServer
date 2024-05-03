#include "doctest.h"
#include "ProtoUtil.h"
#include "CPop3ProtoCmd.h"
#include "user_info.h"
#include <ostream>
#include <fstream>
TEST_CASE("proto util test") {
    CHECK_EQ(1,1);
    {
        std::string strFirst = " test ";
        CHECK_EQ("test",tiny_email::CProtoUtil::Trim(strFirst));
    }
    {
        std::string strFirst = " test test ";
        CHECK_EQ("test test",tiny_email::CProtoUtil::Trim(strFirst));
    }
     {
        std::string strFirst = " test \r\n";
        CHECK_EQ("test",tiny_email::CProtoUtil::Trim(strFirst));
    }
    {
        std::string strName = "user@test.com";
        std::string strDomain = "smtp.test.com";
        CHECK_EQ(strName,tiny_email::CProtoUtil::CreateUserAddrFromNameAndDomain(strName,strDomain));
    }

    {
        std::string strName = "user";
        std::string strDomain = "smtp.test.com";
        std::string strGoodName = "user@test.com";
        CHECK_EQ(strGoodName,tiny_email::CProtoUtil::CreateUserAddrFromNameAndDomain(strName,strDomain));
    }
    {
        std::string strUserCmd = "USER test2";
        tiny_email::CPop3ProtoReqCmd cmd;
        auto result = tiny_email::CPop3ProtoReqCmd::FromString(strUserCmd, cmd);
        CHECK_EQ(tiny_email::PARSE_POP3_RESULT::PARSE_POP3_SUCCEED,result);
        CHECK_EQ("test2",cmd.GetMsg());
    }

    {
        std::string strConfigContent = R"({
    "domainName":"email.test.com",
    "databaseName":"testemail.db",
    "smtpServer":{
    "ip":"127.0.0.1",
    "port":2125
    },
    "pop3Server":{
    "ip":"127.0.0.1",
    "port":2110
    },
    "imapServer":{
     "ip":"127.0.0.1",
    "port":2134
    }
    }
)";
        std::ofstream outFile("test.cfg");
        outFile.write(strConfigContent.c_str(),strConfigContent.length());
        outFile.close();

        tiny_email::email_server_config config;
        if (tiny_email::ParseConfigFromFile("test.cfg", config))
        {
            CHECK_EQ(config.m_strDomain, "email.test.com");
            CHECK_EQ(config.m_strDataBaseName, "testemail.db");
            CHECK_EQ(config.m_smtpServer.strIp_, "127.0.0.1");
            CHECK_EQ(config.m_smtpServer.port_, 2125);
            CHECK_EQ(config.m_pop3Server.strIp_, "127.0.0.1");
            CHECK_EQ(config.m_pop3Server.port_, 2110);
            CHECK_EQ(config.m_imapServer.strIp_, "127.0.0.1");
            CHECK_EQ(config.m_imapServer.port_, 2134);
        }
        else
        {

        }

    }
}