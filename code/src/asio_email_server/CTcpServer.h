#ifndef _CPP_EMAIL_TCP_SERVER_H_
#define _CPP_EMAIL_TCP_SERVER_H_
#include "thirdLib.h"
#include "CTcpClient.h"
#include "CSmtpServerHandler.h"
#include "CPop3ServerHandler.h"
#include "CImapServerHandler.h"
#include "Log.h"
namespace tiny_email
{
using tcp_server_ptr_t = std::shared_ptr<asio::ip::tcp::acceptor>;
template <class T>
class CTcpServer
{
  public:
    explicit CTcpServer(asio::io_context &ioServer, std::string strIp,int port,
        CDataBaseInterface_SHARED_PTR ptr,const std::string strDomainName) : 
        m_ioService(ioServer),
        m_timer(ioServer),                                                                 
        m_strIp(strIp),
        m_port(port),
        m_dataPtr(ptr),
        m_strDomainName(strDomainName)
        {

        };
    void Start();
    void Stop();

  private:
    void SetTimer();
    void OnTimer();
    void DoAccept();
    asio::io_context &m_ioService;
    asio::steady_timer m_timer;
    tcp_server_ptr_t m_accpter;
    using PTR_T = std::shared_ptr<T>;
    std::vector<PTR_T> m_clients;
    std::string m_strIp;
    int m_port;
    std::string m_strDomainName;
    CDataBaseInterface_SHARED_PTR m_dataPtr;
};

template <class T>
void CTcpServer<T>::Start()
{
    asio::ip::address address;
    address.from_string(m_strIp);
    asio::ip::tcp::endpoint endPt(address, m_port);
    m_accpter = std::make_shared<asio::ip::tcp::acceptor>(m_ioService, endPt);
    SetTimer();
    DoAccept();
}

template <class T>
void CTcpServer<T>::Stop()
{

}

template <class T>
void CTcpServer<T>::OnTimer()
{
    SetTimer();
   tiny_email::Info("CTcpServer On Timer");
}

template <class T>
void CTcpServer<T>::SetTimer()
{
    m_timer.expires_from_now(std::chrono::seconds(6));
    m_timer.async_wait([this](std::error_code ec) {
        if (!ec)
        {
            OnTimer();
        }
    });
}

template <class T>
void CTcpServer<T>::DoAccept()
{
    m_accpter->async_accept([this](std::error_code ec, asio::ip::tcp::socket sock) {
        if (!ec)
        {
            auto handler = std::make_shared<T>(m_dataPtr,m_strDomainName);
            auto client = std::make_shared<CTcpClient>(m_ioService, std::move(sock), handler);
            handler->SetTcpSocket(client);
            m_clients.push_back(handler);
            handler->Start();
            DoAccept();
        }
    });
} // namespace cpp_email
using CSmtpServer = CTcpServer<CSmtpHandler>;
using CSmtpServer_SHARED_PTR = std::shared_ptr<CSmtpServer>;

using CImapServer = CTcpServer<CImapServerHandler>;
using CImapServer_SHARED_PTR = std::shared_ptr<CImapServer>;

using CPop3Server = CTcpServer<CPop3Handler>;
using CPop3Server_SHARED_PTR = std::shared_ptr<CPop3Server>;
} // namespace cpp_email
#endif