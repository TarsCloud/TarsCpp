cmake_minimum_required(VERSION 2.8)

project(tars_cpp)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/")
set(CMAKE_VERBOSE_MAKEFILE off)

#set(MYSQL_DIR_INC "/usr/local/mysql/include")
#set(MYSQL_DIR_LIB "/usr/local/mysql/lib")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O2 -Wall -Wno-deprecated")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O2 -Wall -Wno-deprecated")

#for coverage statistics
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O2 -Wall -Wno-deprecated -fprofile-arcs -ftest-coverage")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O2 -Wall -Wno-deprecated -fprofile-arcs -ftest-coverage")

#set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "set build type to release default")
IF (CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release")
ENDIF()

message("CMAKE_BUILD_TYPE TYPE = " ${CMAKE_BUILD_TYPE})

#编译的可执行程序输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(TARS_VERSION "1.4.0")
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


IF (UNIX)
#set(LIB_MYSQL "mysqlclient")
set(INSTALL_PREFIX "/usr/local/tars/cpp")
ELSE()
set(INSTALL_PREFIX "c:\\taf\\cpp")
#set(LIB_MYSQL "libmysql")
ENDIF()

set(CMAKE_INSTALL_PREFIX ${INSTALL_PREFIX})

IF (WIN32)
set(MYSQL_PATH "${CMAKE_SOURCE_DIR}/thirdparty/win_64")
include_directories("${MYSQL_PATH}/include/mysql")
link_directories("${MYSQL_PATH}/lib")
link_libraries("libmysql")

ELSE()

find_package(MySQL REQUIRED)
add_definitions(-DMySQL)
include_directories(${MYSQL_INCLUDE_DIR})
list(APPEND LIB_MYSQL ${MYSQL_LIBRARIES})
link_libraries(${LIB_MYSQL})
link_libraries(pthread dl)

ENDIF()


IF (APPLE)
link_libraries(iconv)
ENDIF(APPLE)


IF (UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11  -Wno-deprecated -fno-strict-aliasing -Wno-overloaded-virtual")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-builtin-macro-redefined -D__FILE__='\"$(notdir $(abspath $<))\"'")
    
    set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -Wall -g")
    set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall -fno-strict-aliasing")

    IF(APPLE)
        message("mac.....")
        SET(CMAKE_C_ARCHIVE_CREATE   "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
        SET(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
        SET(CMAKE_C_ARCHIVE_FINISH   "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
        SET(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
#        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmismatched-tags -Wself-assign")
    ENDIF(APPLE)

ELSEIF (WIN32)
    message("window.....")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4101 /wd4244 /wd4996 /wd4091 /wd4503 /wd4819 /wd4200 /wd4800")
    # set(BUILD_USE_64BITS on)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj " )
    #set(CMAKE_CXX_FLAGS_RELEASE "/GL" )

ELSE ()
    MESSAGE(STATUS "================ ERROR: This platform is unsupported!!! ================")
ENDIF (UNIX)


IF(WIN32)
set(TARS2CPP "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/tars2cpp.exe")
ELSE()
set(TARS2CPP "${CMAKE_BINARY_DIR}/bin/tars2cpp")
ENDIF()


IF(WIN32)
include_directories(${CMAKE_SOURCE_DIR}/util/src/epoll_windows)
ENDIF()

include("${CMAKE_SOURCE_DIR}/cmake/BuildTarsServer.cmake")

include_directories(${CMAKE_SOURCE_DIR}/util/include)
include_directories(${CMAKE_SOURCE_DIR}/servant)
include_directories(${CMAKE_SOURCE_DIR}/servant/protocol)

add_subdirectory(util)
add_subdirectory(tools)
add_subdirectory(servant)
add_subdirectory(examples)

#add_subdirectory(test_deprecated)
