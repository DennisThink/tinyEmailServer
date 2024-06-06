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
		std::string strCmdConnectRsp = handler->GetResponse();
		CHECK_EQ(strCmdConnectRsp, "220  Anti-spam GT for Coremail System\r\n");
	}
	{
		std::string strCmdEHLO = "EHLO smtp.test.com\r\n";
		CHECK(handler->OnClientReq(strCmdEHLO));
		std::string strCmdEhloRsp = handler->GetResponse();
		std::string strCmdEhloStdRsp = "250 OK\r\n";
		strCmdEhloStdRsp += "250-AUTH LOGIN PLAIN\r\n";
		strCmdEhloStdRsp += "250 8BITMIME\r\n";
		CHECK_EQ(strCmdEhloRsp,strCmdEhloStdRsp);
	}
    {
        std::string strCmdAuthLogin = "AUTH LOGIN\r\n";
        CHECK(handler->OnClientReq(strCmdAuthLogin));
        std::string strCmdAuthLoginRsp = handler->GetResponse();
        CHECK_EQ(strCmdAuthLoginRsp, "334 VXNlcm5hbWU6 \r\n");
    }
	{
		std::string strCmdUserName = tiny_email::CProtoUtil::Base64Encode("test")+" \r\n";
		CHECK(handler->OnClientReq(strCmdUserName));
		std::string strCmdUseNameRsp = handler->GetResponse();
		CHECK_EQ(strCmdUseNameRsp, "334 UGFzc3dvcmQ6 \r\n");
	}

	{
		std::string strCmdPassword = tiny_email::CProtoUtil::Base64Encode("test") + " \r\n";
		CHECK(handler->OnClientReq(strCmdPassword));
		std::string strCmdPasswordRsp = handler->GetResponse();
		CHECK_EQ(strCmdPasswordRsp, "235 Authentication successful\r\n");
	}

	{
		std::string strCmdMailFrom = "MAIL FROM: <test@test.com>\r\n";;
		CHECK(handler->OnClientReq(strCmdMailFrom));
		std::string strCmdMailFromRsp = handler->GetResponse();
		CHECK_EQ(strCmdMailFromRsp, "250 Mail Ok\r\n");
	}

	{
		std::string strCmdRcptTo = "RCPT TO : <test1@test.com>\r\n";
		CHECK(handler->OnClientReq(strCmdRcptTo));
		std::string strCmdRcptToRsp = handler->GetResponse();
		CHECK_EQ(strCmdRcptToRsp, "250 Mail Ok\r\n");
	}

	{
		std::string strCmdDataHead = "DATA\r\n";;
		CHECK(handler->OnClientReq(strCmdDataHead));
		std::string strCmdDataHeadRsp = handler->GetResponse();
		CHECK_EQ(strCmdDataHeadRsp, "354 End data with  <CR> <LF>. <CR> <LF>\r\n");
	}

	{
		std::string strCmdDataBody;
		{
			strCmdDataBody += "From: <test@test.com>\r\n";
			strCmdDataBody += "To: <test1@test.com>\r\n";
			strCmdDataBody += "Cc: test2@test.com\r\n";
			strCmdDataBody += "Subject: test \r\n";
			strCmdDataBody += "\r\n";
			strCmdDataBody += "this is a test mail\r\n";
			strCmdDataBody += "\r\n.\r\n";
		}
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