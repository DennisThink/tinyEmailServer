cmake_minimum_required(VERSION 3.0)
project(emailServer)
include_directories(./include/)
include_directories(./include/common/)
include_directories(./include/smtp/)
include_directories(./include/thirdparty/socket-cpp/Socket/)
include_directories(./include/common/config/)
include_directories(./include/database/)
include_directories(./include/thirdparty/)
include_directories(./include/thirdparty/sqlite/)
SET(CMAKE_CXX_FLAGS "-g")
SET(NET_FILE ./include/thirdparty/socket-cpp/Socket/Socket.cpp 
./include/thirdparty/socket-cpp/Socket/TCPServer.cpp )
SET(SMTP_FILE ./src/smtp_server.cpp ./include/smtp/smtp_server_handler.cpp ./include/smtp/CProtoCmd.cpp ./include/common/ProtoUtil.cpp)
SET(SQLITE_CPP_FILE ./include/thirdparty/SQLiteCpp/src/Backup.cpp  
                    ./include/thirdparty/SQLiteCpp/src/Column.cpp 
                    ./include/thirdparty/SQLiteCpp/src/Database.cpp 
                    ./include/thirdparty/SQLiteCpp/src/Exception.cpp 
                    ./include/thirdparty/SQLiteCpp/src/Savepoint.cpp 
                    ./include/thirdparty/SQLiteCpp/src/Statement.cpp 
                    ./include/thirdparty/SQLiteCpp/src/Transaction.cpp)
SET(SQLITE_FILE ./include/thirdparty/sqlite/sqlite3.c)
SET(SQLITE_DB_FILE ./include/database/SqliteDataBase.cpp)
add_executable(emailServer ./src/main.cpp ${NET_FILE} ${SMTP_FILE})
add_executable(database_demo ./src/data_base_demo.cpp ${SQLITE_DB_FILE} ${SQLITE_CPP_FILE} ${SQLITE_FILE})