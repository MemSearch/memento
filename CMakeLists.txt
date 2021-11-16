cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)

project(memento)

add_library(searcher_request STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/searcher_request/sources/searcher_request.cpp)
target_include_directories(searcher_request PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/searcher_request/include)
add_library(trie STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/searcher_request/sources/trie.cpp)
target_include_directories(trie PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/searcher_request/include)

add_executable(main main.cpp)
