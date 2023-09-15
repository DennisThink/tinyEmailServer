#include "net_handler.h"
static int g_socket=-1;
CDataHandler::CDataHandler()
{

}
bool CDataHandler::ConnectToServer(const std::string strServerIp,unsigned short port)
{
    return false;
}
bool CDataHandler::SendData(const std::string strData)
{
    return false;
}
bool CDataHandler::ReciveData(std::string& strData)
{
    return false;
}
bool CDataHandler::Close()
{

}
CDataHandler::~CDataHandler()
{

}