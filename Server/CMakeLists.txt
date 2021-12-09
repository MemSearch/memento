cmake_minimum_required(VERSION 3.16)
project(server_client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "-lpthread") #-lboost_system -lboost_thread  -lpthread

find_package(PostgreSQL REQUIRED)
add_library(searcher_request ${CMAKE_CURRENT_SOURCE_DIR}/searcher_request/sources/searcher_request.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/searcher_request/sources/trie.cpp)
target_include_directories(searcher_request PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/searcher_request/include
        ${PostgreSQL_INCLUDE_DIRS})
target_link_libraries(searcher_request PUBLIC
        ${PostgreSQL_LIBRARIES})

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(server ${CMAKE_CURRENT_SOURCE_DIR}/server.cpp)

target_link_libraries(server ${Boost_LIBRARIES})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/json)
target_link_libraries(server PRIVATE nlohmann_json::nlohmann_json searcher_request)



find_package(PostgreSQL REQUIRED)

add_library(lib STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/searcher_request/sources/searcher_request.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/searcher_request/sources/trie.cpp)
target_include_directories(lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/searcher_request/include
        ${PostgreSQL_INCLUDE_DIRS})

target_link_libraries(lib PRIVATE
        ${PostgreSQL_LIBRARIES})

add_executable(main main.cpp)
target_link_libraries(main searcher_request)


target_include_directories(main PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/searcher_request/include)


# get_target_property(JSON_INC_PATH jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)
# include_directories(${JSON_INC_PATH})

# target_link_libraries(${PROJECT_NAME} jsoncpp_lib)
