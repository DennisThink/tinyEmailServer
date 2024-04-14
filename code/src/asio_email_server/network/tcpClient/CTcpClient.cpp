#include "CTcpClient.h"
#include "Log.h"
namespace tiny_email
{
    CTcpClient::CTcpClient(asio::io_context& service,const INetWorkHandler_SHARED_PTR handler):
    m_ioService(service),
    m_socket(service),
    m_handler(handler)
    {

    }

    bool CTcpClient::ConnectTo(const std::string& domainName,const std::string& protoType)
    {
        using namespace asio;
        ip::tcp::resolver resolver(m_ioService);
        ip::tcp::resolver::query query(domainName, protoType);
        auto iter = resolver.resolve(query);
        auto self = shared_from_this();
        asio::async_connect(m_socket,iter,[self,this](std::error_code ec,asio::ip::tcp::endpoint pt){
            //m_log->info("RemoteAddr:{} {}",pt.address().to_string(),pt.port());
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
            //m_log->info("RemoteAddr:{} {}",pt.address().to_string(),pt.port());
            this->HandleConnect(ec);
        });
        return true;
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
                    //this->m_log->info("Send:{} Length:{}",strValue,length);
                }
                if(m_bShouldClose)
                {
                    m_socket.close();
                }
            });
            return true;
        }
        return false;
    }

    void CTcpClient::DoRead()
    {
        memset(m_recvBuf,0,256);
        auto self = shared_from_this();
        m_socket.async_read_some(asio::buffer(m_recvBuf,255),[this,self](std::error_code ec,std::size_t length){
            //if(m_log)
            {
                //m_log->info("Recv:{}",std::string(m_recvBuf,length));
            }
            if(!ec)
            {
                auto handler = m_handler.lock();
                if(handler)
                {
                    handler->OnRecive(std::string(m_recvBuf,length));
                }
                DoRead();
            }
            else
            {
                Close();
            }
        });
    }
	
    void CTcpClient::HandleConnect(const asio::error_code& ec)
    {
        if(!ec)
        {
            //m_log->info("Connect To Server Succeed");
            m_bConnected = true;
            auto handler = m_handler.lock();
            if(handler)
            {
                handler->OnConnected();
            }
            DoRead();
        }
        else
        {
            //m_log->info("Connect To Server Failed,{}",ec.message());
            m_bConnected = false;
        }
    }

    void CTcpClient::Close()
    {
        m_bShouldClose = true;
        m_bConnected = false;
    }
}