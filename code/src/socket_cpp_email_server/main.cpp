#include <iostream>
#include "smtp_server.h"
#include "pop3_server.h"
#include <string.h>
int main(int argc,char * argv[])
{
    if(argc > 1)
    {
        if(strcmp(argv[1],"smtp")==0)
        {
             StartSmtpServer();
        }
        else
        {
            StartPop3Server();
        }
    }
    std::cout<<"tinyEmailServer"<<std::endl;
   
 
    return 0;
}