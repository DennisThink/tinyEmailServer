#include "doctest.h"
#include "ProtoUtil.h"
#include "../imap/CImapProtoCmd.h"
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