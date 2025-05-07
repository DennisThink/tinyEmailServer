#ifndef _CPP_EMAIL_NET_WORK_HANDER_H_
#define _CPP_EMAIL_NET_WORK_HANDER_H_
#include <memory>
#include <string>
namespace tiny_email
{
   /**
    * @brief 
    * 
    */
   class INetWorkHandler
   {
    public:
       virtual void OnConnected()=0;
       virtual void OnSend()=0;
       virtual void OnRecive(const std::string strValue)=0;
       virtual void OnClose()=0;
       virtual void OnTimer()=0;
       virtual bool IsConnected()=0;
       virtual bool IsFinished() = 0;
   }; 
   using INetWorkHandler_WEAK_PTR = std::weak_ptr<INetWorkHandler>;
   using INetWorkHandler_SHARED_PTR = std::shared_ptr<INetWorkHandler>;
} // cpp_email


#endif