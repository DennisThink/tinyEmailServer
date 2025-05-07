#include "doctest.h"
#include "ProtoUtil.h"
#include "CPop3ProtoCmd.h"
#include "pop3_server_handler.h"
#include "CMemDataBase.h"
#include <memory>
#include <memory.h>

TEST_CASE("pop3_proto_parse") {
	{
		std::string strCmdUser = "USER test\r\n";
		tiny_email::CPop3ProtoReqCmd reqCmd;
		tiny_email::PARSE_POP3_RESULT result = tiny_email::CPop3ProtoReqCmd::FromString(strCmdUser, reqCmd);
		CHECK_EQ(result, tiny_email::PARSE_POP3_RESULT::PARSE_POP3_SUCCEED);
		CHECK_EQ(reqCmd.GetCode(), tiny_email::POP3_CMD_t::POP3_CMD_USER_NAME);
	}

	{
		std::string strCmdPassowrd = "PASS test\r\n";
		tiny_email::CPop3ProtoReqCmd reqCmd;
		tiny_email::PARSE_POP3_RESULT result = tiny_email::CPop3ProtoReqCmd::FromString(strCmdPassowrd, reqCmd);
		CHECK_EQ(result, tiny_email::PARSE_POP3_RESULT::PARSE_POP3_SUCCEED);
		CHECK_EQ(reqCmd.GetCode(), tiny_email::POP3_CMD_t::POP3_CMD_PASS_WORD);
	}

	{
		std::string strCmdStat = "STAT\r\n";
		tiny_email::CPop3ProtoReqCmd reqCmd;
		tiny_email::PARSE_POP3_RESULT result = tiny_email::CPop3ProtoReqCmd::FromString(strCmdStat, reqCmd);
		CHECK_EQ(result, tiny_email::PARSE_POP3_RESULT::PARSE_POP3_SUCCEED);
		CHECK_EQ(reqCmd.GetCode(), tiny_email::POP3_CMD_t::POP3_CMD_STAT);
	}

	{
		std::string strCmdList = "LIST\r\n";
		tiny_email::CPop3ProtoReqCmd reqCmd;
		tiny_email::PARSE_POP3_RESULT result = tiny_email::CPop3ProtoReqCmd::FromString(strCmdList, reqCmd);
		CHECK_EQ(result, tiny_email::PARSE_POP3_RESULT::PARSE_POP3_SUCCEED);
		//CHECK_EQ(reqCmd.GetCode(), tiny_email::Pop3Code_t::POP3_SEND_USER_NAME);
	}

	{
		std::string strCmdRETR = "RETR 1\r\n";
		tiny_email::CPop3ProtoReqCmd reqCmd;
		tiny_email::PARSE_POP3_RESULT result = tiny_email::CPop3ProtoReqCmd::FromString(strCmdRETR, reqCmd);
		CHECK_EQ(result, tiny_email::PARSE_POP3_RESULT::PARSE_POP3_SUCCEED);
		//CHECK_EQ(reqCmd.GetCode(), tiny_email::Pop3Code_t::POP3_SEND_USER_NAME);
	}

	{
		std::string strCmdQuit = "quit\r\n";
		tiny_email::CPop3ProtoReqCmd reqCmd;
		tiny_email::PARSE_POP3_RESULT result = tiny_email::CPop3ProtoReqCmd::FromString(strCmdQuit, reqCmd);
		CHECK_EQ(result, tiny_email::PARSE_POP3_RESULT::PARSE_POP3_SUCCEED);
		//CHECK_EQ(reqCmd.GetCode(), tiny_email::Pop3Code_t::POP3_SEND_USER_NAME);
	}
}
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
		CHECK_EQ(strUserRsp, "+OK cpp mail user name\r\n");
	}

	{
		std::string strCmdPassowrd = "PASS test\r\n";
		CHECK(handler->OnClientReq(strCmdPassowrd));
		std::string strPassowordRsp = handler->GetResponse();
		CHECK_EQ(strPassowordRsp, "+OK 1 message(s) [102 byte(s)]\r\n");
	}

	{
		std::string strCmdStat = "STAT\r\n";
		CHECK(handler->OnClientReq(strCmdStat));
		std::string strStatRsp = handler->GetResponse();
		CHECK_EQ(strStatRsp, "+OK 0\r\n");
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
		CHECK_EQ(strRetrRsp, "+OK 1 message(s) [102 byte(s)]\r\n");
	}

	{
		std::string strCmdQuit = "quit\r\n";
		CHECK_FALSE(handler->OnClientReq(strCmdQuit));
		std::string strQuitRsp = handler->GetResponse();
		CHECK_NE(strQuitRsp, "");
	}
}