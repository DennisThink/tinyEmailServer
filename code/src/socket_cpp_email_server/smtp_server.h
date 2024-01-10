#pragma once
#ifndef _CPP_EMAIL_SMTP_SERVER_H_
#define _CPP_EMAIL_SMTP_SERVER_H_
#include "user_info.h"
void StartSmtpServer(const tiny_email::email_server_config& serverCfg);
#endif