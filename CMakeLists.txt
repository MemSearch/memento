cmake_minimum_required(VERSION 3.20)
project(daemon)

set(CMAKE_CXX_STANDARD 20)

# PostgreSQL
find_package(PostgreSQL REQUIRED)
add_library(database SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/daemon/daemon.cpp)
target_include_directories(database
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/daemon
        ${PostgreSQL_INCLUDE_DIRS})
target_link_libraries(database
        PUBLIC ${PostgreSQL_LIBRARIES})

# daemon
add_executable(daemon main.cpp daemon/daemon.cpp)
target_include_directories(daemon PRIVATE daemon)
target_link_libraries(daemon PRIVATE database)
