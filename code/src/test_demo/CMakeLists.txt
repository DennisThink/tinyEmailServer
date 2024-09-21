cmake_minimum_required(VERSION 3.0)
project(test_demo)
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
include_directories(${INC_BASE}/log/)
include_directories(${INC_BASE}/thirdparty/spdlog/include/)
include_directories(${INC_BASE}/thirdparty/fmt/include/)
include_directories(${INC_BASE}/thirdparty/doctest/)
include_directories(${INC_BASE}/database/)
include_directories(${INC_BASE}/common/)
include_directories(${INC_BASE}/common/config/)
include_directories(${INC_BASE}/proto/pop3/)
include_directories(${INC_BASE}/proto/smtp/)
include_directories(${INC_BASE}/proto/imap/)
include_directories(${INC_BASE}/database/)
include_directories(${INC_BASE}/network/)
include_directories(${INC_BASE}/network/tcpClient/)
include_directories(${INC_BASE}/thirdparty/)
include_directories(${INC_BASE}/thirdparty/sqlite/)

SET(SQLITE_CPP_FILE ${INC_BASE}/thirdparty/SQLiteCpp/src/Backup.cpp  
${INC_BASE}/thirdparty/SQLiteCpp/src/Column.cpp 
${INC_BASE}/thirdparty/SQLiteCpp/src/Database.cpp 
${INC_BASE}/thirdparty/SQLiteCpp/src/Exception.cpp 
${INC_BASE}/thirdparty/SQLiteCpp/src/Savepoint.cpp 
${INC_BASE}/thirdparty/SQLiteCpp/src/Statement.cpp 
${INC_BASE}/thirdparty/SQLiteCpp/src/Transaction.cpp)
SET(SQLITE_FILE ${INC_BASE}/thirdparty/sqlite/sqlite3.c)
SET(DATA_BASE_FILE ${SQLITE_CPP_FILE} ${SQLITE_FILE} ${INC_BASE}/database/SqliteDataBase.cpp)
SET(PROTO_FILE ${INC_BASE}/common/ProtoUtil.cpp ${INC_BASE}/proto/pop3/CPop3ProtoCmd.cpp ${INC_BASE}/proto/imap/CImapProtoCmd.cpp)
add_executable(test_demo 
				./test_demo.cpp 
				./proto_util_test.cpp 
				./database_test.cpp 
				./imap_proto_test.cpp
				./smtp_proto_test.cpp
				${INC_BASE}/proto/smtp/smtp_server_handler.cpp
				./pop3_proto_test.cpp
				${INC_BASE}/proto/imap/imap_server_handler.cpp
				${INC_BASE}/proto/pop3/pop3_server_handler.cpp
				${INC_BASE}/database/CMemDataBase.cpp
				${DATA_BASE_FILE}  
				${PROTO_FILE} 
				${INC_BASE}/common/config/user_info.cpp)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bin)