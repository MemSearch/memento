cmake_minimum_required(VERSION 3.20)
project(daemon)

set(CMAKE_CXX_STANDARD 17)

# sqlite3
find_package(Threads)
add_library(database SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/SQLite3/sqlite3.c)
target_include_directories(database
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/SQLite3)
target_link_libraries(database
        PUBLIC Threads::Threads
        dl)

# daemon
add_executable(daemon main.cpp daemon/daemon.cpp)
target_include_directories(daemon PUBLIC daemon)
target_link_libraries(daemon database)

# gtest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
        test_daemon
        test.cpp
        daemon/daemon.cpp
)

target_include_directories(test_daemon PUBLIC daemon)

target_link_libraries(
        test_daemon
        gtest_main
        database
)

include(GoogleTest)
gtest_discover_tests(test_daemon)
