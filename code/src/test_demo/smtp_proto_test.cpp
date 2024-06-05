#include "doctest.h"
#include "ProtoUtil.h"
#include "smtp_server_handler.h"
#include "CMemDataBase.h"
#include <memory>
#include <memory.h>
TEST_CASE("smtp_proto_test") {
	std::string strDomain = "test.com";
	auto db_ptr = std::make_shared<tiny_email::CMemDataBase>();
	db_ptr->AddUser("test", "test");
	auto handler = std::make_shared<tiny_email::CSmtpServerProtoHandler>(db_ptr, strDomain);	
	{
		std::string strCmdEHLO = "EHLO smtp.test.com\r\n";
		CHECK(handler->OnClientReq(strCmdEHLO));
		std::string strCmdEhloRsp = handler->GetResponse();
		CHECK_EQ(strCmdEhloRsp, "");
	}
    {
        std::string strCmdAuthLogin = "AUTH LOGIN\r\n";
        CHECK(handler->OnClientReq(strCmdAuthLogin));
        std::string strCmdAuthLoginRsp = handler->GetResponse();
        CHECK_EQ(strCmdAuthLoginRsp, "");
    }
	{
		std::string strCmdUserName = tiny_email::CProtoUtil::Base64Encode("test")+" \r\n";
		CHECK(handler->OnClientReq(strCmdUserName));
		std::string strCmdUseNameRsp = handler->GetResponse();
		CHECK_EQ(strCmdUseNameRsp, "");
	}

	{
		std::string strCmdPassword = tiny_email::CProtoUtil::Base64Encode("test") + " \r\n";
		CHECK(handler->OnClientReq(strCmdPassword));
		std::string strCmdPasswordRsp = handler->GetResponse();
		CHECK_EQ(strCmdPasswordRsp, "");
	}

	{
		std::string strCmdMailFrom = "MAIL FROM: <test@test.com>\r\n";;
		CHECK(handler->OnClientReq(strCmdMailFrom));
		std::string strCmdMailFromRsp = handler->GetResponse();
		CHECK_EQ(strCmdMailFromRsp, "");
	}

	{
		std::string strCmdRcptTo = "RCPT TO : <test1@test.com>\r\n";
		CHECK(handler->OnClientReq(strCmdRcptTo));
		std::string strCmdRcptToRsp = handler->GetResponse();
		CHECK_EQ(strCmdRcptToRsp, "");
	}

	{
		std::string strCmdDataHead = "DATA\r\n";;
		CHECK(handler->OnClientReq(strCmdDataHead));
		std::string strCmdDataHeadRsp = handler->GetResponse();
		CHECK_EQ(strCmdDataHeadRsp, "");
	}

	{
		std::string strCmdDataBody = "MAIL FROM: <test1@test.com>\r\n";;
		CHECK(handler->OnClientReq(strCmdDataBody));
		std::string strCmdDataBodyRsp = handler->GetResponse();
		CHECK_EQ(strCmdDataBodyRsp, "");
	}

	{
		std::string strCmdDataTail = "\r\n.\r\n";;
		CHECK(handler->OnClientReq(strCmdDataTail));
		std::string strCmdDataTailRsp = handler->GetResponse();
		CHECK_EQ(strCmdDataTailRsp, "");
	}
}