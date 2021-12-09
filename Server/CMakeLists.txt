cmake_minimum_required(VERSION 3.16)
project(server_client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS " -lboost_system -lboost_thread  -lpthread ")

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(server ${CMAKE_CURRENT_SOURCE_DIR}/server.cpp)

target_link_libraries(server ${Boost_LIBRARIES})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/json)
target_link_libraries(server PRIVATE nlohmann_json::nlohmann_json)
