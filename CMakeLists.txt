cmake_minimum_required(VERSION 3.16)
project(site)

set(CMAKE_EXE_LINKER_FLAGS "-pthread -Wall -Wextra -Werror")

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(main main.cpp)
target_link_libraries(main ${Boost_LIBRARIES})

add_library(_server STATIC ${PROJECT_SOURCE_DIR}/lib/server.cpp)
target_include_directories(_server PUBLIC ${PROJECT_SOURCE_DIR}/lib)

add_library(_client STATIC ${PROJECT_SOURCE_DIR}/lib/client.cpp)
target_include_directories(_client PUBLIC ${PROJECT_SOURCE_DIR}/lib)

add_library(_memInfo STATIC ${PROJECT_SOURCE_DIR}/lib/memInfo.cpp)
target_include_directories(_memInfo PUBLIC ${PROJECT_SOURCE_DIR}/lib)

target_link_libraries(main _server _client _memInfo)

# gtest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(mem_tests tests/main_test.cpp)
target_include_directories(mem_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_link_libraries(mem_tests gtest_main _memInfo)

include(GoogleTest)
gtest_discover_tests(mem_tests)
