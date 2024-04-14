#ifndef _CPP_EMAIL_C_TCP_CLIENT_H_
#define _CPP_EMAIL_C_TCP_CLIENT_H_
#include "thirdLib.h"
#include "networkHandler.h"
namespace tiny_email
{
    class CTcpClient final:public std::enable_shared_from_this<CTcpClient>
    {
    public:
        explicit CTcpClient(asio::io_context& service,
                            const INetWorkHandler_SHARED_PTR handler);
        explicit CTcpClient(asio::io_context& service,
                            asio::ip::tcp::socket sock,
                            const INetWorkHandler_SHARED_PTR handler):
                            m_ioService(service),
                            m_socket(std::move(sock)),
                            m_handler(handler)
                            {
                               
                                m_bConnected = true;
                            }
        void Start(){DoRead();};
        bool ConnectTo(const std::string& domainName,const std::string& protoType);
        bool ConnectTo(const std::string& ip,const int Port);
        bool Send(const std::string& strValue);
        bool Receive(std::string& strValue);
        void HandleConnect(const asio::error_code& ec);
        bool isConnected()const {return m_bConnected;}
        void Close();
    protected:
        void DoRead();
    private:
        bool m_bConnected = false;
        bool m_bShouldClose = false;
        asio::io_context& m_ioService;
        asio::ip::tcp::socket m_socket;
        INetWorkHandler_WEAK_PTR m_handler;
        char m_sendBuf[256];
        char m_recvBuf[256];
    };
    using CTcpClient_ptr_t = std::shared_ptr<CTcpClient>;
} // cpp_email

#endif