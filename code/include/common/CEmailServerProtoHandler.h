#ifndef _C_EMAIL_SERVER_PROTO_HANDLER_H_
#define _C_EMAIL_SERVER_PROTO_HANDLER_H_
#include "DataBaseInterface.h"
#include <string.h>
namespace tiny_email
{
    class CEmailServerProtoHandlerInterface
    {
    public:
        CEmailServerProtoHandlerInterface(CDataBaseInterface_SHARED_PTR dbPtr, const std::string strDomainName):m_db(dbPtr),m_strDomainName(strDomainName){}
        virtual bool OnClientReq(const std::string strValue)=0;
        virtual std::string GetResponse()=0;
        virtual bool IsFinished() = 0;
        virtual ~CEmailServerProtoHandlerInterface()=default;
    protected:
        CDataBaseInterface_SHARED_PTR m_db;
        std::string m_strDomainName;
    };

    using CEmailServerProtoInterface_PTR = std::shared_ptr<CEmailServerProtoHandlerInterface>;
}
#endif