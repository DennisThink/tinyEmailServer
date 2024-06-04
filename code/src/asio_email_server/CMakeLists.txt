cmake_minimum_required(VERSION 3.5)
project(emailServer)
set(CMAKE_CXX_STANDARD 20)
if(WIN32)
add_definitions(-DWINDOWS)
add_compile_options(/W3)
#add_compile_options(/WX)
else()
add_compile_definitions(-Wall) 
SET(CMAKE_CXX_FLAGS "-g")
link_libraries(pthread)
endif()
SET(INC_BASE ../../include)
include_directories(${INC_BASE}/database/)
include_directories(${INC_BASE}/database/)
include_directories(${INC_BASE}/thirdparty/asio/)
include_directories(${INC_BASE}/thirdparty/)
include_directories(${INC_BASE}/thirdparty/spdlog/include/)
include_directories(${INC_BASE}/thirdparty/sqlite/)
include_directories(${INC_BASE}/thirdparty/json11/)
include_directories(${INC_BASE}/thirdparty/clipp/include/)
include_directories(${INC_BASE}/common/)
include_directories(${INC_BASE}/common/config/)
include_directories(${INC_BASE}/common/util/)
include_directories(${INC_BASE}/proto/)
include_directories(${INC_BASE}/proto/code/)
include_directories(${INC_BASE}/proto/smtp/)
include_directories(${INC_BASE}/proto/pop3/)
include_directories(${INC_BASE}/proto/imap/)
include_directories(${INC_BASE}/log/)
include_directories(${INC_BASE}/network/)
include_directories(${INC_BASE}/network/tcpClient/)
include_directories(${INC_BASE}/handler/)
include_directories(.)
SET(SERVED_BUILD_STATIC 1)
if(WIN32)
add_definitions(-DWINDOWS)
else()
link_libraries(pthread)
endif()
SET(EMAIL_SERVER CEmailServer.h CEmailServer.cpp 
                 CTcpServer.h
                 ${INC_BASE}/handler/CImapServerHandler.h  
                 ${INC_BASE}/handler/CImapServerHandler.cpp
                 ${INC_BASE}/handler/CSmtpServerHandler.h  
                 ${INC_BASE}/handler/CSmtpServerHandler.cpp
                 ${INC_BASE}/handler/CPop3ServerHandler.h  
                 ${INC_BASE}/handler/CPop3ServerHandler.cpp
                 ${INC_BASE}/handler/CNetServerHandler.h
                 ${INC_BASE}/proto/smtp/smtp_server_handler.cpp 
                 ${INC_BASE}/proto/pop3/pop3_server_handler.cpp
                 ${INC_BASE}/proto/imap/imap_server_handler.cpp
                 ${INC_BASE}/proto/pop3/CPop3ProtoCmd.cpp
                 ${INC_BASE}/proto/imap/CImapProtoCmd.cpp
                 ${INC_BASE}/network/tcpClient/CTcpClient.h
                 ${INC_BASE}/network/tcpClient/CTcpClient.cpp
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