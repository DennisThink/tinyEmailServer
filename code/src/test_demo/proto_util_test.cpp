#include "doctest.h"
#include "ProtoUtil.h"
TEST_CASE("proto util test") {
    CHECK_EQ(1,1);
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
}