cmake_minimum_required(VERSION 2.18)
project(memento)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(Boost_USE_MULTITHREADED ON)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(CURL)

# Add Telegram API
add_subdirectory(third_party/tgbot-cpp)

# SQLite3
add_library(db SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/SQLite3/sqlite3.c)
target_include_directories(db PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/SQLite3)
target_link_libraries(db
        PUBLIC Threads::Threads
        dl)

# bot
add_library(img_url_bot SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/src/tg_bot.cpp)
target_include_directories(img_url_bot PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OPENSSL_INCLUDE_DIR}
        ${Boost_INCLUDE_DIR})
if (CURL_FOUND)
    target_include_directories(img_url_bot PUBLIC
            ${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()
target_link_libraries(img_url_bot
        img_storage
        TgBot
        ${CMAKE_THREAD_LIBS_INIT}
        ${OPENSSL_LIBRARIES}
        ${Boost_LIBRARIES}
        ${CURL_LIBRARIES})

# img_storage
add_library(img_storage SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/src/img_url_db.cpp)
target_include_directories(img_storage
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(img_storage
        db)

#Tests
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest)
enable_testing()