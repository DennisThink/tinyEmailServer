#include "doctest.h"
#include "ProtoUtil.h"
#include "CImapProtoCmd.h"
#include "imap_server_handler.h"
#include "CMemDataBase.h"
#include <memory>
#include <memory.h>
TEST_CASE("proto util test") {
	using namespace tiny_email;
	//CAPABILITY Good
	{
		std::string strCapa = "A01 CAPABILITY\r\n";
		CImapProtoCmd cmd = CImapProtoCmd::FromString(strCapa);
		CHECK_EQ(cmd.GetCmdId(), "A01");
		CHECK_EQ(cmd.GetCode(),ImapCode_t::IMAP_CMD_CAPABILITY);
	}

	{
		std::string strCapa = "A02 NOOP\r\n";
		CImapProtoCmd cmd = CImapProtoCmd::FromString(strCapa);
		CHECK_EQ(cmd.GetCmdId(), "A02");
		CHECK_EQ(cmd.GetCode(), ImapCode_t::IMAP_CMD_NOOP);
	}

	{
		std::string strCapa = "A03 LOGIN USERNAME password\r\n";
		CImapProtoCmd cmd = CImapProtoCmd::FromString(strCapa);
		CHECK_EQ(cmd.GetCmdId(), "A03");
		CHECK_EQ(cmd.GetCode(), ImapCode_t::IMAP_CMD_LOGIN);
	}

	{
		std::string strCapa = "A04 SELECT INBOX\r\n";
		CImapProtoCmd cmd = CImapProtoCmd::FromString(strCapa);
		CHECK_EQ(cmd.GetCmdId(), "A04");
		CHECK_EQ(cmd.GetCode(), ImapCode_t::IMAP_CMD_SELECT);
	}

	{
		std::string strCapa = "A05 examine inbox\r\n";
		CImapProtoCmd cmd = CImapProtoCmd::FromString(strCapa);
		CHECK_EQ(cmd.GetCmdId(), "A05");
		CHECK_EQ(cmd.GetCode(), ImapCode_t::IMAP_CMD_EXAMINE);
	}

	{
		std::string strCapa = "A07 SEARCH ALL\r\n";
		CImapProtoCmd cmd = CImapProtoCmd::FromString(strCapa);
		CHECK_EQ(cmd.GetCmdId(), "A07");
		CHECK_EQ(cmd.GetCode(), ImapCode_t::IMAP_CMD_SEARCH);
	}

	{
		std::string strCapa = "A08 FETCH 7 RFC822\r\n";
		CImapProtoCmd cmd = CImapProtoCmd::FromString(strCapa);
		CHECK_EQ(cmd.GetCmdId(), "A08");
		CHECK_EQ(cmd.GetCode(), ImapCode_t::IMAP_CMD_FETCH);
	}

	{
		std::string strCapa = "A06 LIST "" *\r\n";
		CImapProtoCmd cmd = CImapProtoCmd::FromString(strCapa);
		CHECK_EQ(cmd.GetCmdId(), "A06");
		CHECK_EQ(cmd.GetCode(), ImapCode_t::IMAP_CMD_LIST);
	}

	{
		std::string strCapa = "A09 LOGOUT\r\n";
		CImapProtoCmd cmd = CImapProtoCmd::FromString(strCapa);
		CHECK_EQ(cmd.GetCmdId(), "A09");
		CHECK_EQ(cmd.GetCode(), ImapCode_t::IMAP_CMD_LOGOUT);
	}
}

TEST_CASE("imap_recv_email_test") {
	std::string strDomain = "test.com";
	auto db_ptr = std::make_shared<tiny_email::CMemDataBase>();
	db_ptr->AddUser("test", "test");
	auto handler = std::make_shared<tiny_email::CImapServerProtoHandler>(db_ptr, strDomain);
	{
		{
			std::string strCmdA1 = "A1 CAPABILITY\r\n";
			CHECK(handler->OnClientReq(strCmdA1));
			std::string strRspA1 = handler->GetResponse();
			CHECK_EQ(strRspA1, " * CAPABILITY IMAP4 IMAP4rev1 XLIST MOVE IDLE XAPPLEPUSHSERVICE AUTH=PLAIN AUTH=LOGIN AUTH=XOAUTH2 NAMESPACE CHILDREN ID UIDPLUS\r\n A01 OK CAPABILITY Completed\r\n");
		}

		{
			std::string strCmdA2 = "A02 NOOP\r\n";
			CHECK(handler->OnClientReq(strCmdA2));
			std::string strRspA2 = handler->GetResponse();
			std::string strStdRspA2 = "";
			CHECK_EQ(strRspA2, "");
		}

		{
			std::string strCmdA3 = "A03 LOGIN USERNAME password\r\n";
			CHECK(handler->OnClientReq(strCmdA3));
			std::string strRspA3 = handler->GetResponse();
			CHECK_EQ(strRspA3, " A03 OK Success login ok\r\n");
		}

		{
			std::string strCmdA4 = "A04 SELECT INBOX\r\n";
			CHECK(handler->OnClientReq(strCmdA4));
			std::string strRspA4 = handler->GetResponse();
			CHECK_EQ(strRspA4, "");
		}

		{
			std::string strCmdA5 = "A05 examine inbox\r\n";
			CHECK(handler->OnClientReq(strCmdA5));
			std::string strRspA5 = handler->GetResponse();
			CHECK_EQ(strRspA5, "");
		}

		{
			std::string strCmdA6 = "A06 LIST "" *\r\n";
			CHECK(handler->OnClientReq(strCmdA6));
			std::string strRspA6 = handler->GetResponse();
			CHECK_EQ(strRspA6, "");
		}

		{
			std::string strCmdA7 = "A07 SEARCH ALL\r\n";
			CHECK(handler->OnClientReq(strCmdA7));
			std::string strRspA7 = handler->GetResponse();
			CHECK_EQ(strRspA7, "");
		}

		{
			std::string strCmdA8 = "A08 FETCH 1 RFC822\r\n";
			CHECK(handler->OnClientReq(strCmdA8));
			std::string strRspA8 = handler->GetResponse();
			CHECK_EQ(strRspA8, "");
		}

		{
			std::string strCmdA9 = "A09 LOGOUT\r\n";
			CHECK(handler->OnClientReq(strCmdA9));
			std::string strRspA9 = handler->GetResponse();
			CHECK_EQ(strRspA9, "");
		}
	}
}