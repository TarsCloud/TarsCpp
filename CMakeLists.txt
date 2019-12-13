cmake_minimum_required(VERSION 2.8)

project(tars_cpp)

set(CMAKE_VERBOSE_MAKEFILE off)

set(MYSQL_DIR_INC "/usr/local/mysql/include")
set(MYSQL_DIR_LIB "/usr/local/mysql/lib")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O2 -Wall -Wno-deprecated")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O2 -Wall -Wno-deprecated")

#for coverage statistics
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O2 -Wall -Wno-deprecated -fprofile-arcs -ftest-coverage")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O2 -Wall -Wno-deprecated -fprofile-arcs -ftest-coverage")

#set(CMAKE_BUILD_TYPE "Debug")

set(TARS_VERSION "1.2.0")
add_definitions(-DTARS_VERSION="${TARS_VERSION}")
set(TARS_SSL 0)
add_definitions(-DTARS_SSL=${TARS_SSL})
set(TARS_HTTP2 0)
add_definitions(-DTARS_HTTP2=${TARS_HTTP2})

set(_USE_OPENTRACKING $ENV{_USE_OPENTRACKING})
if(_USE_OPENTRACKING)
set(OPENTRACKING_INC "/usr/local/include")
add_definitions(-D_USE_OPENTRACKING=${_USE_OPENTRACKING})
endif()

set(INSTALL_PREFIX "/usr/local/tars/cpp")

set(CMAKE_INSTALL_PREFIX ${INSTALL_PREFIX})

add_subdirectory(util)
add_subdirectory(tools)

set(TARS2CPP "${tools_BINARY_DIR}/tars2cpp/tars2cpp")

add_subdirectory(servant)

