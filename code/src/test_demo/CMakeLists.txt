cmake_minimum_required(VERSION 3.0)
project(test_demo)
set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_FLAGS "-g")
include_directories(../../include/thirdparty/doctest/)
include_directories(../../include/common/)
include_directories(../../include/common/config/)
include_directories(../../include/pop3/)
include_directories(../../include/database/)
include_directories(../../include/thirdparty/)
include_directories(../../include/thirdparty/sqlite/)
SET(SQLITE_CPP_FILE ../../include/thirdparty/SQLiteCpp/src/Backup.cpp  
../../include/thirdparty/SQLiteCpp/src/Column.cpp 
../../include/thirdparty/SQLiteCpp/src/Database.cpp 
../../include/thirdparty/SQLiteCpp/src/Exception.cpp 
../../include/thirdparty/SQLiteCpp/src/Savepoint.cpp 
../../include/thirdparty/SQLiteCpp/src/Statement.cpp 
../../include/thirdparty/SQLiteCpp/src/Transaction.cpp)
SET(SQLITE_FILE ../../include/thirdparty/sqlite/sqlite3.c)
SET(DATA_BASE_FILE ${SQLITE_CPP_FILE} ${SQLITE_FILE} ../../include/database/SqliteDataBase.cpp)
SET(PROTO_FILE ../../include/common/ProtoUtil.cpp ../../include/pop3/CPop3ProtoCmd.cpp ../../include/imap/CImapProtoCmd.cpp)
add_executable(test_demo ./test_demo.cpp ./proto_util_test.cpp ./database_test.cpp ./imap_proto_test.cpp ${DATA_BASE_FILE}  ${PROTO_FILE})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bin) 