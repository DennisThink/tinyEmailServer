#ifndef _CPP_EMAIL_C_PROTO_CODE_H_
#define _CPP_EMAIL_C_PROTO_CODE_H_
namespace tiny_email
{
    enum ProtoCode_t:int{
        CODE_BAD_CMD_1=1,
        CODE_ANY=2,
        CODE_250=250,
        CODE_220=220,
        CODE_334=334,
        CODE_235=235,
        CODE_354=354,
        CODE_HELO,
    };

    enum class PARSE_SMTP_RESULT:int {
        PARSE_SMTP_SUCCEED,
        PARSE_SMTP_SYNTAX_ERROR,
        PARSE_SMTP_PARAM_ERROR,
    };

    enum class SMTP_CMD_t:int{
        SMTP_CMD_NO_CMD,
        SMTP_CMD_HELO_EHLO,
        SMTP_CMD_AUTH,
        SMTP_CMD_MAIL_FROM,
        SMTP_CMD_RECP_TO,
        SMTP_CMD_DATA,
        SMTP_CMD_VERY,
        SMTP_CMD_EXPN,
        SMTP_CMD_HELP,
        SMTP_CMD_QUIT,
        SMTP_CMD_RSET,
    };



    enum class PARSE_POP3_RESULT:int {
        PARSE_POP3_SUCCEED,
        PARSE_POP3_SYNTAX_ERROR,
        PARSE_POP3_FAILED,
    };
/*
const std::string strPop3SendUser="USER dennismi\r\n";
const std::string strPop3SendPasswd="PASS srpPr9MXwp285Su\r\n";
const std::string strPop3SendStat ="STAT\r\n";
const std::string strPop3SendUidl ="UIDL 1\r\n";
const std::string strPop3SendRetr ="RETR 1\r\n";
const std::string strPop3SendNoop ="NOOP\r\n";
const std::string strPop3SendDel  ="DELE 1\r\n";
const std::string strPop3SendList ="LIST\r\n";
const std::string strPop3SendQuit ="QUIT\r\n";*/
    enum class POP3_CMD_t:int{
        POP3_CMD_NO_CMD,
        POP3_CMD_USER_NAME,
        POP3_CMD_PASS_WORD,
        POP3_CMD_STAT,
        POP3_CMD_UIDL,
        POP3_CMD_RETR,
        POP3_CMD_NOOP,
        POP3_CMD_DELE,
        POP3_CMD_LIST,
        POP3_CMD_QUIT,
        POP3_USER_NAME_RSP,
        POP3_PASS_WORD_RSP,
    };

    enum class POP3_STEP_t:int {
        POP3_STEP_BEGIN,
        POP3_STEP_SERVER_ON_CONNECT,
        POP3_STEP_SERVER_SEND_USER_NAME_OK,
        POP3_STEP_SERVER_SEND_USER_NAME_BAD,
        POP3_STEP_SERVER_SEND_PASS_WORD_OK,
        POP3_STEP_SERVER_SEND_PASS_WORD_BAD,
        POP3_STEP_SERVER_SEND_STAT_OK,
        POP3_STEP_SERVER_SEND_UIDL_OK,
        POP3_STEP_SERVER_SEND_RETR_OK,
        POP3_STEP_SERVER_SEND_NOOP_OK,
        POP3_STEP_SERVER_SEND_DELE_OK,
        POP3_STEP_SERVER_SEND_LIST_OK,
        POP3_STEP_SERVER_SEND_TOP_OK,
        POP3_STEP_SERVER_SEND_QUIT_OK,
        POP3_STEP_END,
    };
    
    enum Pop3Code_t:int{
        POP3_OK,
        POP3_BAD,
        POP3_VERIFY,
        POP3_PROCESS,
        POP3_UPDATE,
        POP3_SEND_USER_NAME,
        POP3_SEND_PASS_WORD,
        POP3_SHOW_FIRST_EMAIL,
        POP3_SHOW_FIRST_EMAIL_ID,
        POP3_RECEIVE_FIRST_EMAIL,
        POP3_QUIT,
        POP3_NO_OP,
    };

    enum class PARSE_IMAP_RESULT:int {
        PARSE_IMAP_SUCCEED,
        PARSE_IMAP_SYNTAX_ERROR,
        PARSE_IMAP_FAILED,
    };

    enum IMAP_CMD_t
    {
        IMAP_CMD_CAPABILITY,
        IMAP_CMD_NOOP,
        IMAP_CMD_LOGIN,
        IMAP_CMD_SELECT,
        IMAP_CMD_EXAMINE,
        IMAP_CMD_CREATE,
        IMAP_CMD_RENAME,
        IMAP_CMD_DELETE,
        IMAP_CMD_SUBSCRIBE,
        IMAP_CMD_UN_SUBSCRIBE,
        IMAP_CMD_LSUB,
        IMAP_CMD_LIST,
        IMAP_CMD_FETCH,
        IMAP_CMD_LOGOUT,
        IMAP_CMD_UNKNOWN_CMD,
    };

    enum IMAP_STEP_t
    {
        IMAP_STEP_SERVER_ON_CONNECTED,
        IMAP_STEP_SERVER_SEND_CAPALIBITY,
        IMAP_STEP_SERVER_SEND_LOGIN_SUCCEED,
        IMAP_STEP_SERVER_SEND_LOGIN_FAILED,
        IMAP_STEP_SERVER_SEND_NOOP,
        IMAP_STEP_SERVER_SEND_SELECT,
        IMAP_STEP_SERVER_SEND_EXAMINE,
        IMAP_STEP_SERVER_SEND_CREATE,
        IMAP_STEP_SERVER_SEND_RENAME,
        IMAP_STEP_SERVER_SEND_DELETE,
        IMAP_STEP_SERVER_SEND_SUBSCRIBE,
        IMAP_STEP_SERVER_SEND_UN_SUBSCRIBE,
        IMAP_STEP_SERVER_SEND_LSUB,
        IMAP_STEP_SERVER_SEND_LIST,
        IMAP_STEP_SERVER_SEND_FETCH,
        IMAP_STEP_SERVER_SEND_LOGOUT,
        IMAP_STEP_SERVER_SEND_BAD_PARSE_CMD_ERROR,
    };
    enum ImapCode_t
    {
        IMAP_OK,
        IMAP_BAD,
        IMAP_WAIT_VERIFY,
        IMAP_SEND_VERIFY,
        IMAP_SEND_LIST,
        IMAP_NO_OP,
    };
    enum Smtp_Server_Step_t
    {
       SMTP_BEGIN,
       SMTP_ON_CONNECT,
       SMTP_RECV_HELO_FIRST,
       SMTP_RECV_AUTH_LOGIN_REQ,
       SMTP_RECV_PASS_WORD_REQ,
       SMTP_NAME_PASS_VERIFY,
       SMTP_RECV_MAIL_FROM_REQ,
       SMTP_RECV_RCPT_TO_REQ,
       SMTP_RECV_DATA_REQ,
       SMTP_END,
    };
    enum Smtp_Step_t
    {
       SMTP_STEP_BEGIN,
       RECEIVE_220,
       SEND_HELO,
       SEND_AUTH_LOGIN,
       SEND_USER_NAME,
       SEND_PASSWORD,
       AUTH_SUCCESS,
       SEND_MAIL_FROM,
       SEND_MAIL_TO,
       SEND_DATA_HEAD,
       SEND_DATA_BODY,
       SEND_DATA_TAIL,
       SEND_FINISH, 
       SEND_NO_OP, 
       
    };
}
#endif