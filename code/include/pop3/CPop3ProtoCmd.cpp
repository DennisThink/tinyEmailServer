#include "CPop3ProtoCmd.h"
#include "ProtoUtil.h"
#include <algorithm>
#include <iostream>
namespace tiny_email
{
    CPop3ProtoCmd CPop3ProtoCmd::FromString(const std::string& strValue)
    {
        {
            auto okPos = strValue.find_first_of("+OK");
            if(okPos != std::string::npos)
            {
                return CPop3ProtoCmd(Pop3Code_t::POP3_OK,strValue.substr(okPos,strValue.length()-okPos-1),false);
            }
        }
        {
            auto errPos = strValue.find_first_of("-ERR");
            if(errPos != std::string::npos)
            {
                return CPop3ProtoCmd(Pop3Code_t::POP3_BAD,strValue.substr(errPos,strValue.length()-errPos-1),false);
            }
        }
        return CPop3ProtoCmd(Pop3Code_t::POP3_BAD,"UnKnown Cmd",false);
    }

    PARSE_POP3_RESULT CPop3ProtoReqCmd::FromString(const std::string& strValue,CPop3ProtoReqCmd& cmd)
    {
        auto posSpace = strValue.find(" ");
        if(posSpace != std::string::npos)
        {
            std::string strPrefix = strValue.substr(0,posSpace);
            std::transform(strPrefix.begin(),strPrefix.end(),strPrefix.begin(),::toupper);
            std::string strMessage = strValue.substr(posSpace,strValue.length()-posSpace-2);
            std::cout<<"Msg   "<<strMessage<<std::endl;
            strMessage = CProtoUtil::Trim(strMessage);
            /*const std::string strPop3SendUser="USER dennismi\r\n";
            const std::string strPop3SendPasswd="PASS srpPr9MXwp285Su\r\n";
            const std::string strPop3SendStat ="STAT\r\n";
            const std::string strPop3SendUidl ="UIDL 1\r\n";
            const std::string strPop3SendRetr ="RETR 1\r\n";
            const std::string strPop3SendNoop ="NOOP\r\n";
            const std::string strPop3SendDel  ="DELE 1\r\n";
            const std::string strPop3SendList ="LIST\r\n";
            const std::string strPop3SendQuit ="QUIT\r\n";*/
            if(strPrefix=="USER")
            {
                cmd = CPop3ProtoReqCmd(POP3_CMD_t::POP3_CMD_USER_NAME,strMessage);
                return PARSE_POP3_RESULT::PARSE_POP3_SUCCEED;
            }
            else if(strPrefix == "PASS")
            {
                cmd = CPop3ProtoReqCmd(POP3_CMD_t::POP3_CMD_PASS_WORD,strMessage);
                return PARSE_POP3_RESULT::PARSE_POP3_SUCCEED;
            }
            else if(strPrefix == "UIDL")
            {
                cmd = CPop3ProtoReqCmd(POP3_CMD_t::POP3_CMD_UIDL,strMessage);
                return PARSE_POP3_RESULT::PARSE_POP3_SUCCEED;
            }
            else if(strPrefix == "RETR")
            {
                cmd = CPop3ProtoReqCmd(POP3_CMD_t::POP3_CMD_RETR,strMessage);
                return PARSE_POP3_RESULT::PARSE_POP3_SUCCEED;
            }
            else if(strPrefix == "DELE")
            {
                cmd = CPop3ProtoReqCmd(POP3_CMD_t::POP3_CMD_DELE,strMessage);
                return PARSE_POP3_RESULT::PARSE_POP3_SUCCEED;
            }
        }
        std::string strPrefix = strValue;
        std::transform(strPrefix.begin(),strPrefix.end(),strPrefix.begin(),::toupper);

        /*const std::string strPop3SendStat ="STAT\r\n";    
        const std::string strPop3SendNoop ="NOOP\r\n";
        const std::string strPop3SendList ="LIST\r\n";
        const std::string strPop3SendQuit ="QUIT\r\n";*/
        if(strPrefix=="QUIT\r\n")
        {
            cmd = CPop3ProtoReqCmd(POP3_CMD_t::POP3_CMD_QUIT,"");
            return PARSE_POP3_RESULT::PARSE_POP3_SUCCEED;
        }
        else if(strPrefix == "NOOP\r\n")
        {
            cmd = CPop3ProtoReqCmd(POP3_CMD_t::POP3_CMD_NOOP,"");
            return PARSE_POP3_RESULT::PARSE_POP3_SUCCEED;
        }
        else if(strPrefix == "LIST\r\n")
        {
            cmd = CPop3ProtoReqCmd(POP3_CMD_t::POP3_CMD_LIST,"");
            return PARSE_POP3_RESULT::PARSE_POP3_SUCCEED;
        }
        else
        {
            return PARSE_POP3_RESULT::PARSE_POP3_FAILED;
        }
    }

}