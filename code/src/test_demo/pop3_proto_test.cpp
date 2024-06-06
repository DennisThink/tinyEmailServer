#include "doctest.h"
#include "ProtoUtil.h"
#include "CPop3ProtoCmd.h"
#include "pop3_server_handler.h"
#include "CMemDataBase.h"
#include <memory>
#include <memory.h>
TEST_CASE("pop3_proto_test") {
	std::string strDomain = "test.com";
	auto db_ptr = std::make_shared<tiny_email::CMemDataBase>();
	db_ptr->AddUser("test", "test");
	auto handler = std::make_shared<tiny_email::CPop3ServerProtoHandler>(db_ptr, strDomain);
	{
		std::string strUserRsp = handler->GetResponse();
		CHECK_EQ(strUserRsp, "+OK Welcome to test.com\r\n");
	}
	{
		std::string strCmdUser = "USER test\r\n";
		CHECK(handler->OnClientReq(strCmdUser));
		std::string strUserRsp = handler->GetResponse();
		CHECK_EQ(strUserRsp, " +OK cpp mail user name\r\n");
	}

	{
		std::string strCmdPassowrd = "PASS test\r\n";
		CHECK(handler->OnClientReq(strCmdPassowrd));
		std::string strPassowordRsp = handler->GetResponse();
		CHECK_EQ(strPassowordRsp, "");
	}

	{
		std::string strCmdStat = "STAT\r\n";
		CHECK(handler->OnClientReq(strCmdStat));
		std::string strStatRsp = handler->GetResponse();
		CHECK_EQ(strStatRsp, "");
	}

	{
		std::string strCmdList = "LIST\r\n";
		CHECK(handler->OnClientReq(strCmdList));
		std::string strListRsp = handler->GetResponse();
		CHECK_EQ(strListRsp, "");
	}

	{
		std::string strCmdRETR = "RETR 1\r\n";
		CHECK(handler->OnClientReq(strCmdRETR));
		std::string strRetrRsp = handler->GetResponse();
		CHECK_EQ(strRetrRsp, "");
	}

	{
		std::string strCmdQuit = "quit\r\n";
		CHECK(handler->OnClientReq(strCmdQuit));
		std::string strQuitRsp = handler->GetResponse();
		CHECK_EQ(strQuitRsp, "");
	}
}