#ifndef _C_POP3_PROTO_CMD_H_
#define _C_POP3_PROTO_CMD_H_

#include <string>
#include "CProtoCode.h"
#include <memory>
namespace tiny_email
{
    class CPop3ProtoCmd
    {
    public:
        static CPop3ProtoCmd FromString(const std::string& strValue);
        Pop3Code_t GetCode() const 
        {
            return m_code;
        }

        std::string GetMessage() const 
        {
            return m_message;
        }
        
        bool isFinish() const 
        {
            return m_bFinish;
        }
    private:
        explicit CPop3ProtoCmd(Pop3Code_t code,std::string message,bool bFinish)
        {
            m_code = code;
            m_message = message;
            m_bFinish = bFinish;
        }
        Pop3Code_t m_code;
        std::string m_message;
        bool m_bFinish=false;
    };

    class CPop3ProtoReqCmd
    {
    public:
        explicit CPop3ProtoReqCmd()
        {
            m_code = POP3_CMD_t::POP3_CMD_NO_CMD;
            m_message="";
        }
        static PARSE_POP3_RESULT FromString(const std::string& strValue,CPop3ProtoReqCmd& cmd);
        POP3_CMD_t GetCode() const 
        {
            return m_code;
        }

        std::string GetMessage() const 
        {
            return m_message;
        }
        
    private:
        explicit CPop3ProtoReqCmd(POP3_CMD_t code,std::string message)
        {
            m_code = code;
            m_message = message;
        }
        POP3_CMD_t m_code;
        std::string m_message;
    };

    class CPop3RspBase
    {
    public:
        explicit CPop3RspBase()=default;
        virtual bool Valid()const{return false;}
        virtual std::string GetString()=0;
        virtual bool FromString(const std::string& strValue)=0;
        virtual POP3_CMD_t GetCmd()=0;
    };

    class CPop3UserNameRsp:public CPop3RspBase
    {
    public:
        explicit CPop3UserNameRsp()=default;
        virtual bool Valid()const{return false;}
        virtual std::string GetString(){
            return "+OK Core Mail\r\n";
        };
        virtual bool FromString(const std::string& strValue){
            if(strValue.find("+OK")!=strValue.npos)
            {
                m_result=PARSE_POP3_RESULT::PARSE_POP3_SUCCEED;
                return true;
            }
            if(strValue.find("-ERR") != strValue.npos)
            {
                 m_result=PARSE_POP3_RESULT::PARSE_POP3_FAILED;
                return true;
            }
            return false;
        };
        virtual POP3_CMD_t GetCmd(){return POP3_CMD_t::POP3_USER_NAME_RSP;}
        virtual PARSE_POP3_RESULT GetResult(){return m_result;}
    private:
        POP3_CMD_t m_cmd;
        PARSE_POP3_RESULT m_result;

    };

    class CPop3PasswordRsp:public CPop3RspBase
    {
    public:
        explicit CPop3PasswordRsp()=default;
        virtual bool Valid()const{return false;}
        virtual std::string GetString()=0;
        virtual bool FromString(const std::string& strValue)
        {
            if(strValue.find("+OK")!=strValue.npos)
            {
                m_result=PARSE_POP3_RESULT::PARSE_POP3_SUCCEED;
                return true;
            }
            if(strValue.find("-ERR") != strValue.npos)
            {
                 m_result=PARSE_POP3_RESULT::PARSE_POP3_FAILED;
                return true;
            }
            return false;
        }
        virtual POP3_CMD_t GetCmd(){return POP3_CMD_t::POP3_USER_NAME_RSP;}
        virtual PARSE_POP3_RESULT GetResult(){return m_result;};
    private:
        POP3_CMD_t m_cmd;
        PARSE_POP3_RESULT m_result;
    };
    class CPop3ReqBase
    {
    public:
        explicit CPop3ReqBase()=default;
        virtual bool Valid()const{return false;}
        virtual std::string GetString()=0;
        virtual bool FromString(const std::string& strValue)=0;
        virtual POP3_CMD_t GetCmd()=0;
    };
    using CPop3ReqBase_PTR_T = std::shared_ptr<CPop3ReqBase>;
    
    class CPop3UserNameReq:public CPop3ReqBase
    {
    public:
        explicit CPop3UserNameReq(const std::string& strUserName){m_userName =  strUserName;}
        virtual std::string GetString(){return "USER "+m_userName+"\r\n";}
        virtual bool FromString(const std::string& strValue){
            if(strValue.empty())
            {
                return false;
            }
            return true;
        };
        virtual POP3_CMD_t GetCmd() override {
            return POP3_CMD_t::POP3_CMD_USER_NAME;
        }

        std::string GetUserName() const {
            return m_userName;
        }
    private:
        std::string m_userName;
    };

    class CPop3PasswordReq:public CPop3ReqBase
    {
    public:
        explicit CPop3PasswordReq(const std::string& strUserName){m_password =  strUserName;}
        virtual std::string GetString(){return "PASS "+m_password+"\r\n";}
        virtual bool FromString(const std::string& strValue){
            if(strValue.empty())
            {
                return false;
            }
            return true;
        };
        virtual POP3_CMD_t GetCmd() override {
            return POP3_CMD_t::POP3_CMD_PASS_WORD;
        }

        std::string GetPassword() const {
            return m_password;
        }
    private:
        std::string m_password;
    };
}
#endif