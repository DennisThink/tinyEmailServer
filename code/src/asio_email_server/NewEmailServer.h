#ifndef _CPP_EMAIL_NEW_EMAIL_SERVER_H_
#define _CPP_EMAIL_NEW_EMAIL_SERVER_H_
#include "CNetServerHandler.h"
#include "../imap/imap_server_handler.h"
#include "../smtp/smtp_server_handler.h"
#include "../pop3/pop3_server_handler.h"
namespace tiny_email
{

	using CSmtpServeHandlerNew = CNetServerHandler<CSmtpServerProtoHandler>;
	using CSmtpServerNew = CTcpServer<CSmtpServeHandlerNew> ;
	using CSmtpServerNew_SHARED_PTR = std::shared_ptr<CSmtpServerNew>;

	using CPop3ServerHandlerNew = CNetServerHandler<CPop3ServerProtoHandler>;
	using CPop3ServerNew = CTcpServer<CPop3ServerHandlerNew>;
	using CPop3ServerNew_SHARED_PTR = std::shared_ptr<CPop3ServerNew>;

	using CImapServerHandlerNew = CNetServerHandler<CImapServerProtoHandler>;
	using CImapServerNew = CTcpServer<CImapServerHandlerNew>;
	using CImapServerNew_SHARED_PTR = std::shared_ptr<CImapServerNew>;
}
#endif