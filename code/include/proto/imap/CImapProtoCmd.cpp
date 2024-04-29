#include "CImapProtoCmd.h"
#include "ProtoUtil.h"
#include <string.h>
#include <stdio.h>
namespace tiny_email
{
	#ifdef WINDOWS
	#define STRICMP stricmp
	#else
	#define STRICMP strcasecmp
	#endif
	CImapProtoCmd CImapProtoCmd::FromString(const std::string& strValue)
	{
		std::size_t startIndex = 0;
		auto index = strValue.find_first_of(" ");
		std::string strCmdId;
		std::string strCmd;
		std::string strParam;
		if (index != std::string::npos)
		{
			strCmdId = strValue.substr(startIndex, index-startIndex);
		}

		startIndex = index+1;
		index = strValue.find_first_of(" ", startIndex);
		if (index != std::string::npos)
		{
			strCmd = strValue.substr(startIndex, index - startIndex);
			startIndex = index+1;
			strParam = strValue.substr(index, strValue.length() - startIndex);
		}
		else
		{
			strCmd = strValue.substr(startIndex, strValue.length() - startIndex-2);
		}

		{
			ImapCode_t imapCode = ImapCode_t::IMAP_CMD_UNKNOWN_CMD;
			if (STRICMP(strCmd.c_str(), "CAPABILITY") == 0)
			{
				imapCode = ImapCode_t::IMAP_CMD_CAPABILITY;
			}
			if (STRICMP(strCmd.c_str(), "NOOP") == 0)
			{
				imapCode = ImapCode_t::IMAP_CMD_NOOP;
			}
			if (STRICMP(strCmd.c_str(), "SELECT") == 0)
			{
				imapCode = ImapCode_t::IMAP_CMD_SELECT;
			}
			if (STRICMP(strCmd.c_str(), "EXAMINE") == 0)
			{
				imapCode = ImapCode_t::IMAP_CMD_EXAMINE;
			}
			if (STRICMP(strCmd.c_str(), "SEARCH") == 0)
			{
				imapCode = ImapCode_t::IMAP_CMD_SEARCH;
			}
			if (STRICMP(strCmd.c_str(), "FETCH") == 0)
			{
				imapCode = ImapCode_t::IMAP_CMD_FETCH;
			}
			if (STRICMP(strCmd.c_str(), "LIST") == 0)
			{
				imapCode = ImapCode_t::IMAP_CMD_LIST;
			}
			if (STRICMP(strCmd.c_str(), "LOGOUT") == 0)
			{
				imapCode = ImapCode_t::IMAP_CMD_LOGOUT;
			}
			if (STRICMP(strCmd.c_str(), "LOGIN") == 0)
			{
				imapCode = ImapCode_t::IMAP_CMD_LOGIN;
			}

			return CImapProtoCmd(strCmdId,imapCode, strParam,false);
		}
	}
}