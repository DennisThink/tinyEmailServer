cmake_minimum_required(VERSION 3.0)
project(socket_cpp_email_server)
set(CMAKE_CXX_STANDARD 20)
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
message("Windows")
add_definitions(-DWINDOWS)
add_compile_options(/W3)
#add_compile_options(/WX)
else()
SET(CMAKE_CXX_FLAGS "-g")
endif()

SET(INC_BASE ../../include)
include_directories(${INC_BASE})
include_directories(${INC_BASE}/log/)
include_directories(${INC_BASE}/thirdparty/spdlog/include/)
include_directories(${INC_BASE}/thirdparty/fmt/include/)
include_directories(${INC_BASE}/database/)
include_directories(${INC_BASE}/common/)
include_directories(${INC_BASE}/common/config/)
include_directories(${INC_BASE}/proto/pop3/)
include_directories(${INC_BASE}/proto/smtp/)
include_directories(${INC_BASE}/proto/imap/)
include_directories(${INC_BASE}/database/)
include_directories(${INC_BASE}/thirdparty/)
include_directories(${INC_BASE}/thirdparty/sqlite/)
include_directories(${INC_BASE}/thirdparty/socket-cpp/Socket/)
#include_directories(../../include/)
#include_directories(../../include/common/)
#include_directories(../../include/common/config/)

#include_directories(../../include/log/)

#include_directories(../../include/database/)

#include_directories(../../include/thirdparty/)
#include_directories(../../include/thirdparty/sqlite/)
#include_directories(../../include/thirdparty/fmt/include/)
#include_directories(../../include/thirdparty/spdlog/include/)
#include_directories(../../include/thirdparty/socket-cpp/Socket/)

#include_directories(../../include/proto/smtp/)
#include_directories(../../include/proto/pop3/)
#include_directories(../../include/proto/imap/)






SET(NET_FILE ${INC_BASE}/thirdparty/socket-cpp/Socket/Socket.cpp 
${INC_BASE}/thirdparty/socket-cpp/Socket/TCPServer.cpp )
SET(SMTP_FILE 
./smtp_server.cpp 
${INC_BASE}/proto/smtp/smtp_server_handler.cpp 
${INC_BASE}/proto/smtp/CProtoCmd.cpp 
${INC_BASE}/common/ProtoUtil.cpp)

SET(POP3_FILE ./pop3_server.cpp 
${INC_BASE}/proto/pop3/pop3_server_handler.cpp 
${INC_BASE}/common/ProtoUtil.cpp 
${INC_BASE}/proto/pop3/CPop3ProtoCmd.cpp)

SET(SQLITE_CPP_FILE ${INC_BASE}/thirdparty/SQLiteCpp/src/Backup.cpp  
${INC_BASE}/thirdparty/SQLiteCpp/src/Column.cpp 
${INC_BASE}/thirdparty/SQLiteCpp/src/Database.cpp 
${INC_BASE}/thirdparty/SQLiteCpp/src/Exception.cpp 
${INC_BASE}/thirdparty/SQLiteCpp/src/Savepoint.cpp 
${INC_BASE}/thirdparty/SQLiteCpp/src/Statement.cpp 
${INC_BASE}/thirdparty/SQLiteCpp/src/Transaction.cpp)

SET(SQLITE_FILE ${INC_BASE}/thirdparty/sqlite/sqlite3.c)

SET(LOG_FILE ${INC_BASE}/log/LogUtil.cpp)

SET(SQLITE_DB_FILE ${INC_BASE}/database/SqliteDataBase.cpp ${INC_BASE}/common/config/user_info.cpp)

add_executable(socket_cpp_email_server ./main.cpp ${NET_FILE} ${SMTP_FILE} ${POP3_FILE} ${SQLITE_CPP_FILE} ${SQLITE_FILE} ${SQLITE_DB_FILE} ${LOG_FILE})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bin) 