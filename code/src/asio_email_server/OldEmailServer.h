#ifndef _CPP_EMAIL_OLD_EMAIL_SERVER_H_
#define _CPP_EMAIL_OLD_EAMIL_SERVER_H_
#include "CTcpServer.h"
#include "CSmtpServerHandler.h"
#include "CPop3ServerHandler.h"
#include "CImapServerHandler.h"
namespace tiny_email
{

	using CSmtpServer = CTcpServer<CSmtpHandler>;
	using CSmtpServer_SHARED_PTR = std::shared_ptr<CSmtpServer>;

	using CImapServer = CTcpServer<CImapServerHandler>;
	using CImapServer_SHARED_PTR = std::shared_ptr<CImapServer>;

	using CPop3Server = CTcpServer<CPop3Handler>;
	using CPop3Server_SHARED_PTR = std::shared_ptr<CPop3Server>;
}
#endif