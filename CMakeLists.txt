cmake_minimum_required(VERSION 3.20)
project(daemon)

set(CMAKE_CXX_STANDARD 20)

# PostgreSQL + OpenCV + Tesseract
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(TESSERACT REQUIRED tesseract)
find_package(PostgreSQL REQUIRED)

link_directories( ${TESSERACT_LIBRARY_DIRS} )

add_library(libs SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/daemon/daemon.cpp)
target_include_directories(libs PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/daemon
        ${PostgreSQL_INCLUDE_DIRS}
        ${TESSERACT_INCLUDE_DIRS} )
target_link_libraries(libs PUBLIC
        ${PostgreSQL_LIBRARIES}
        ${OpenCV_LIBS}
        ${TESSERACT_LIBRARIES})

# daemon
add_executable(daemon main.cpp daemon/daemon.cpp)
target_include_directories(daemon PRIVATE daemon)
target_link_libraries(daemon PRIVATE libs)

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
