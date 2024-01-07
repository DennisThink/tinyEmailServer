cmake_minimum_required(VERSION 3.0)
project(socket_cpp_email_server)
set(CMAKE_CXX_STANDARD 20)
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
message("Windows")
add_definitions(-DWINDOWS)

else()
endif()
include_directories(../../include/)
include_directories(../../include/common/)
include_directories(../../include/smtp/)
include_directories(../../include/pop3/)
include_directories(../../include/log/)
include_directories(../../include/thirdparty/spdlog/include/)
include_directories(../../include/thirdparty/socket-cpp/Socket/)
include_directories(../../include/common/config/)
include_directories(../../include/database/)
include_directories(../../include/thirdparty/)
include_directories(../../include/thirdparty/sqlite/)
include_directories(../../include/thirdparty/)
include_directories(../../include/thirdparty/fmt/include/)
SET(CMAKE_CXX_FLAGS "-g")
SET(NET_FILE ../../include/thirdparty/socket-cpp/Socket/Socket.cpp 
../../include/thirdparty/socket-cpp/Socket/TCPServer.cpp )
SET(SMTP_FILE ./smtp_server.cpp ../../include/smtp/smtp_server_handler.cpp ../../include/smtp/CProtoCmd.cpp ../../include/common/ProtoUtil.cpp)
SET(POP3_FILE ./pop3_server.cpp ../../include/pop3/pop3_server_handler.cpp ../../include/common/ProtoUtil.cpp ../../include/pop3/CPop3ProtoCmd.cpp)
SET(SQLITE_CPP_FILE ../../include/thirdparty/SQLiteCpp/src/Backup.cpp  
../../include/thirdparty/SQLiteCpp/src/Column.cpp 
../../include/thirdparty/SQLiteCpp/src/Database.cpp 
../../include/thirdparty/SQLiteCpp/src/Exception.cpp 
../../include/thirdparty/SQLiteCpp/src/Savepoint.cpp 
../../include/thirdparty/SQLiteCpp/src/Statement.cpp 
../../include/thirdparty/SQLiteCpp/src/Transaction.cpp)
SET(SQLITE_FILE ../../include/thirdparty/sqlite/sqlite3.c)
SET(LOG_FILE ../../include/log/LogUtil.cpp)
SET(SQLITE_DB_FILE ../../include/database/SqliteDataBase.cpp ../../include/common/config/user_info.cpp)
add_executable(socket_cpp_email_server ./main.cpp ${NET_FILE} ${SMTP_FILE} ${POP3_FILE} ${SQLITE_CPP_FILE} ${SQLITE_FILE} ${SQLITE_DB_FILE} ${LOG_FILE})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bin) 