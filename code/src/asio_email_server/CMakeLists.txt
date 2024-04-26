cmake_minimum_required(VERSION 3.5)
project(emailServer)
set(CMAKE_CXX_STANDARD 20)
SET(INC_BASE ../../include)
include_directories(${INC_BASE}/database/)
include_directories(${INC_BASE}/database/)
include_directories(${INC_BASE}/thirdparty/asio/)
include_directories(${INC_BASE}/thirdparty/)
include_directories(${INC_BASE}/thirdparty/spdlog/include/)
include_directories(${INC_BASE}/thirdparty/sqlite/)
include_directories(${INC_BASE}/thirdparty/json11/)
include_directories(${INC_BASE}/common/)
include_directories(${INC_BASE}/common/config/)
include_directories(${INC_BASE}/common/util/)
include_directories(${INC_BASE}/proto/)
include_directories(${INC_BASE}/proto/code/)
include_directories(${INC_BASE}/proto/smtp/)
include_directories(${INC_BASE}/proto/pop3/)
include_directories(${INC_BASE}/proto/imap/)
include_directories(${INC_BASE}/log/)
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
                 ${INC_BASE}/smtp/smtp_server_handler.cpp 
                 ${INC_BASE}/pop3/pop3_server_handler.cpp
                 ${INC_BASE}/imap/imap_server_handler.cpp
                 ${INC_BASE}/pop3/CPop3ProtoCmd.cpp
                 ${INC_BASE}/imap/CImapProtoCmd.cpp
                 ./network/tcpClient/CTcpClient.h
                 ./network/tcpClient/CTcpClient.cpp
                 ${INC_BASE}/common/ProtoUtil.cpp
                 ${INC_BASE}/common/config/user_info.cpp
                 ${INC_BASE}/log/LogUtil.cpp
                 # DataBase
                 ${INC_BASE}/database/SqliteDataBase.cpp
                 )
SET(SQLITE_CPP_FILE ../../include/thirdparty/SQLiteCpp/src/Backup.cpp  
../../include/thirdparty/SQLiteCpp/src/Column.cpp 
../../include/thirdparty/SQLiteCpp/src/Database.cpp 
../../include/thirdparty/SQLiteCpp/src/Exception.cpp 
../../include/thirdparty/SQLiteCpp/src/Savepoint.cpp 
../../include/thirdparty/SQLiteCpp/src/Statement.cpp 
../../include/thirdparty/SQLiteCpp/src/Transaction.cpp)

SET(SQLITE_FILE ../../include/thirdparty/sqlite/sqlite3.c)
SET(SOURCE_FILE ${EMAIL_SERVER} main.cpp ${SQLITE_CPP_FILE} ${SQLITE_FILE})
add_executable(emailServer ${SOURCE_FILE})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bin) 