cmake_minimum_required(VERSION 3.10)
project(emailServer)
SET(INC_BASE ../../include)
include_directories(${INC_BASE}/database/)
include_directories(${INC_BASE}/database/)
include_directories(${INC_BASE}/thirdparty/asio/)
include_directories(${INC_BASE}/thirdparty/)
include_directories(${INC_BASE}/thirdparty/json11/)
include_directories(${INC_BASE}/common/)
include_directories(${INC_BASE}/common/config/)
include_directories(${INC_BASE}/common/util/)
include_directories(${INC_BASE}/proto/)
include_directories(${INC_BASE}/proto/code/)
include_directories(${INC_BASE}/proto/smtp/)
include_directories(${INC_BASE}/proto/pop3/)
include_directories(${INC_BASE}/proto/imap/)
include_directories(.)
include_directories(./network/)
include_directories(./network/tcpClient/)
include_directories(./handler/)
SET(SERVED_BUILD_STATIC 1)
if(WIN32)
else()
link_libraries(pthread)
endif()
SET(EMAIL_SERVER CEmailServer.h CEmailServer.cpp 
                 CTcpServer.h
                 ./handler/CImapServerHandler.h  
                 ./handler/CImapServerHandler.cpp
                 ./handler/CSmtpServerHandler.h  
                 ./handler/CSmtpServerHandler.cpp
                 ./handler/CPop3ServerHandler.h  
                 ./handler/CPop3ServerHandler.cpp
                 ./network/tcpClient/CTcpClient.h
                 ./network/tcpClient/CTcpClient.cpp
                 ${INC_BASE}/common/ProtoUtil.cpp
                 ${INC_BASE}/common/config/user_info.cpp
                 # DataBase
                 ${INC_BASE}/database/CMemDataBase.cpp
                 )
SET(SOURCE_FILE ${EMAIL_SERVER} main.cpp )
add_executable(emailServer ${SOURCE_FILE})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bin) 