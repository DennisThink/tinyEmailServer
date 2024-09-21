cmake_minimum_required(VERSION 3.0)
project(database_demo)
set(CMAKE_CXX_STANDARD 20)
if(WIN32)

add_definitions(-DWINDOWS)
add_compile_options(/W4)
#add_compile_options(/WX)
else()
add_compile_definitions(-Wall) 
SET(CMAKE_CXX_FLAGS "-g")
link_libraries(pthread)
endif()

SET(INC_BASE ../../include)
include_directories(${INC_BASE}/log/)
include_directories(${INC_BASE}/thirdparty/spdlog/include/)
include_directories(${INC_BASE}/thirdparty/fmt/include/)
include_directories(${INC_BASE}/database/)
include_directories(${INC_BASE}/common/)
include_directories(${INC_BASE}/common/config/)
include_directories(${INC_BASE}/proto/pop3/)
include_directories(${INC_BASE}/database/)
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
add_executable(database_demo ./data_base_demo.cpp  ${DATA_BASE_FILE})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bin)