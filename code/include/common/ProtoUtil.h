#ifndef _CPP_EMAIL_PROTO_UTIL_H_
#define _CPP_EMAIL_PROTO_UTIL_H_
#include <string>
#include <vector>
#include <ctime>
namespace tiny_email
{
    class CProtoUtil final
    {
    public:
        static std::string GetSmtpDomainFromMainDomain(const std::string strMainDomain);
        static std::string GetPop3DomainFromMainDomain(const std::string strMainDomain);
        static std::string GetImapDomainFromMainDomain(const std::string strMainDomain);
        static std::vector<std::string> SplitStringByLine(const std::string& strValue);
        static bool SplitLine(const std::string& strInput,int& code,std::string& value,bool& bFinish);
        static std::string Base64Encode(const std::string strInput);
        static std::string Base64Decode(const std::string strInput);
        static bool ParseFromToString(const std::string strInput,std::string& name,std::string& emailAddr);
        static std::string CreateUserAddrFromNameAndDomain(const std::string strName,const std::string strDomain);
        static std::string CreateUserNameFromAddrAndDomain(const std::string strAddr,const std::string strDomain);
        static std::string Trim(const std::string &s);
        static std::time_t Now();
        static std::string TimeToString(const std::time_t t); 
        static std::string GetUserNameFromEmailAddr(std::string strEmailAddr);
        static std::string CompareString(const std::string strFirst, const std::string strSecond);
    };
}
#endif