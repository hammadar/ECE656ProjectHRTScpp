cmake_minimum_required(VERSION 3.15)
project(ECE656ProjectHRTS)

set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/usr/lib/x86_64-linux-gnu")


include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}}/DatabaseHandling
                    /usr/include/boost)
link_directories(/usr/lib/x86_64-linux-gnu)
find_package(Threads REQUIRED)
find_library(MYSQL_LIBS NAMES libmysqlcppconn.so  REQUIRED)
find_library(BOOST_LIBS NAMES libboost_date_time.so REQUIRED)
add_executable(${PROJECT_NAME} main.cpp DatabaseHandling/databaseConnection.h DatabaseHandling/databaseConnection.cpp DatabaseClasses/socialNetworkBase.h DatabaseClasses/User.cpp DatabaseClasses/User.h Utility/setHandling.cpp Utility/setHandling.h)
target_link_libraries(${PROJECT_NAME} ${MYSQL_LIBS} ${BOOST_LIBS} Threads::Threads)


