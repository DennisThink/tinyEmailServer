#ifndef _TINY_EMAIL_NET_HANDLER_H_
#define _TINY_EMAIL_NET_HANDLER_H_
#include <string>
class CDataHandler
{
public:
    CDataHandler();
    bool ConnectToServer(const std::string strServerIp,unsigned short port);
    bool SendData(const std::string strData);
    bool ReciveData(std::string& strData);
    bool Close();
    virtual ~CDataHandler();
};
#endif