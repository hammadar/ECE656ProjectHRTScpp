cmake_minimum_required(VERSION 3.15)
project(ECE656ProjectHRTS)

set(CMAKE_CXX_STANDARD 20)

set(MYSQL_DIR "/usr/local/mysql-connector-c++")

include_directories(${CMAKE_SOURCE_DIR}
                    ${MYSQL_DIR}/include)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED)
find_library(SSL_LIB NAMES ssl PATHS ${MYSQL_DIR}/lib64 NO_DEFAULT_PATH)
find_library(CRYPTO_LIB NAMES crypto PATHS ${MYSQL_DIR}/lib64 NO_DEFAULT_PATH)
find_library(MYSQL_LIBS NAMES libmysqlcppconn PATHS ${MYSQL_DIR}/lib64)
add_executable(${PROJECT_NAME} main.cpp databaseConnection.h databaseConnection.cpp)
target_link_libraries(${PROJECT_NAME} ${MYSQL_LIBS} ${SSL_LIB} ${CRYPTO_LIB} Threads::Threads)


