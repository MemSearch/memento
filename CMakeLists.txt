cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)

project(memento)

find_package(PostgreSQL REQUIRED)

add_library(searcher_request STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/searcher_request/sources/searcher_request.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/searcher_request/sources/trie.cpp)
target_include_directories(searcher_request PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/searcher_request/include
    ${PostgreSQL_INCLUDE_DIRS})

target_link_libraries(searcher_request PRIVATE
    ${PostgreSQL_LIBRARIES})

add_executable(main main.cpp)
target_link_libraries(main searcher_request)


target_include_directories(main PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/searcher_request/include)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest)
enable_testing()

add_executable(tests_search tests/tests.cpp)

target_include_directories(tests_search PUBLIC
        ${PROJECT_SOURCE_DIR}/searcher_request)

target_link_libraries(tests_search gtest_main searcher_request)
