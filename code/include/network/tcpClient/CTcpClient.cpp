#include "CTcpClient.h"
#include "Log.h"
namespace tiny_email
{
    CTcpClient::CTcpClient(asio::io_context& service,const INetWorkHandler_SHARED_PTR handler):
    m_ioService(service),
    m_socket(service),
    m_handler(handler),
    m_bConnected(false),
    m_bShouldClose(false)
    {
        m_localEndPoint = m_socket.local_endpoint();
        m_RemoteEndPoint = m_socket.remote_endpoint();
        m_bConnected = true;
        ShowEndPointInfo(" Create ");
    }
    CTcpClient::~CTcpClient()
    {
        ShowEndPointInfo(" Destroy ");
    }
    bool CTcpClient::ConnectTo(const std::string& domainName,const std::string& protoType)
    {
        using namespace asio;
        ip::tcp::resolver resolver(m_ioService);
        ip::tcp::resolver::query query(domainName, protoType);
        auto iter = resolver.resolve(query);
        auto self = shared_from_this();
        asio::async_connect(m_socket,iter,[self,this](std::error_code ec,asio::ip::tcp::endpoint pt){
            tiny_email::Info("RemoteAddr:{} {}",pt.address().to_string(),pt.port());
            this->HandleConnect(ec);
        });

        return true;
    }
    
    bool CTcpClient::ConnectTo(const std::string& ip,const int Port)
    {
        using namespace asio;
        ip::tcp::resolver resolver(m_ioService);
        auto iter = resolver.resolve(ip,std::to_string(Port));
        auto self = shared_from_this();
        asio::async_connect(m_socket,iter,[self,this](std::error_code ec,asio::ip::tcp::endpoint pt){
            tiny_email::Info("RemoteAddr:{} {}",pt.address().to_string(),pt.port());
            this->HandleConnect(ec);
        });
        return true;
    }

    void CTcpClient::ShowEndPointInfo(const std::string strInfo)
    {
        tiny_email::Info("[Local]{}:{}--->[Remote]{}:{} {}",
            m_localEndPoint.address().to_string(),
            m_localEndPoint.port(),
            m_RemoteEndPoint.address().to_string(),
            m_RemoteEndPoint.port(),strInfo.c_str());
    }
    bool CTcpClient::Send(const std::string& strValue)
    {
        if(isConnected())
        {
			memset(m_sendBuf,0,256);
            memcpy(m_sendBuf,strValue.c_str(),strValue.length());
            auto self = shared_from_this();
            asio::async_write(m_socket,asio::buffer(m_sendBuf,strValue.length()),[this,self,strValue](std::error_code ec,std::size_t length){
                if(!ec)
                {
                    tiny_email::Info("Send:{} Length:{}",strValue,length);
                }
                else
                {
                    tiny_email::Warn("Send:{} Length:{} Failed", strValue, length);
                    Close();
                }
            });
            return true;
        }
        return false;
    }

    void CTcpClient::DoRead()
    {
        if (isConnected())
        {
            memset(m_recvBuf, 0, 256);
            auto self = shared_from_this();
            m_socket.async_read_some(asio::buffer(m_recvBuf, 255), [this, self](std::error_code ec, std::size_t length) {
                tiny_email::Info("Recv:{}", std::string(m_recvBuf, length));
                if (!ec)
                {
                    auto handler = m_handler.lock();
                    if (handler)
                    {
                        handler->OnRecive(std::string(m_recvBuf, length));
                    }
                    DoRead();
                }
                else
                {
                    Close();
                }
                });
        }
    }
    void CTcpClient::HandleConnect(const asio::error_code& ec)
    {
        if(!ec)
        {
            tiny_email::Info("Connect To Server Succeed");
            m_bConnected = true;
            auto handler = m_handler.lock();
            if(handler)
            {
                handler->OnConnected();
            }
            m_localEndPoint = m_socket.local_endpoint();
            m_RemoteEndPoint = m_socket.remote_endpoint();
            DoRead();
        }
        else
        {
            tiny_email::Info("Connect To Server Failed,{}",ec.message());
            m_bConnected = false;
        }
    }

    void CTcpClient::Close()
    {
        m_bShouldClose = false;
        m_bConnected = false;
        m_socket.close();
    }
}