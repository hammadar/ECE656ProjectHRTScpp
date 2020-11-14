cmake_minimum_required(VERSION 3.15)
project(ECE656ProjectHRTS)

set(CMAKE_CXX_STANDARD 20)

set(MYSQL_DIR "/usr/local/mysql-connector-c++")

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}}/DatabaseHandling
                    ${MYSQL_DIR}/include)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED)
find_library(SSL_LIB NAMES ssl PATHS ${MYSQL_DIR}/lib64 NO_DEFAULT_PATH)
find_library(CRYPTO_LIB NAMES libcrypto.dylib PATHS ${MYSQL_DIR}/lib64 NO_DEFAULT_PATH)
find_library(MYSQL_LIBS NAMES libmysqlcppconn8.2.8.0.22.dylib PATHS ${MYSQL_DIR}/lib64 NO_DEFAULT_PATH)
add_executable(${PROJECT_NAME} main.cpp DatabaseHandling/databaseConnection.h DatabaseHandling/databaseConnection.cpp DatabaseClasses/socialNetworkBase.h DatabaseClasses/User.cpp DatabaseClasses/User.h)
target_link_libraries(${PROJECT_NAME} ${MYSQL_LIBS} ${SSL_LIB} ${CRYPTO_LIB} Threads::Threads)


