cmake_minimum_required(VERSION 3.2)

project(tars_cpp)

set(TARS_VERSION "2.0.0")
add_definitions(-DTARS_VERSION="${TARS_VERSION}")
set(TARS_MYSQL 1)
set(TARS_SSL 0)
set(TARS_HTTP2 0)
set(TARS_PROTOBUF 0)
set(TARS_ZLIB 0)

add_definitions(-DTARS_MYSQL=${TARS_MYSQL})
add_definitions(-DTARS_SSL=${TARS_SSL})
add_definitions(-DTARS_HTTP2=${TARS_HTTP2})
add_definitions(-DTARS_PROTOBUF=${TARS_PROTOBUF})
add_definitions(-DTARS_ZLIB=${TARS_ZLIB})

# list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/")
set(CMAKE_VERBOSE_MAKEFILE off)

#for coverage statistics
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O2 -Wall -Wno-deprecated -fprofile-arcs -ftest-coverage")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O2 -Wall -Wno-deprecated -fprofile-arcs -ftest-coverage")

#set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "set build type to release default")
IF (CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release")
ENDIF()


#编译的可执行程序输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(_USE_OPENTRACKING $ENV{_USE_OPENTRACKING})
if(_USE_OPENTRACKING)
set(OPENTRACKING_INC "/usr/local/include")
add_definitions(-D_USE_OPENTRACKING=${_USE_OPENTRACKING})
endif()

#-------------------------------------------------------------

if("${INSTALL_PREFIX}" STREQUAL "")
IF (UNIX)
set(INSTALL_PREFIX "/usr/local/tars/cpp")
ELSE()
set(INSTALL_PREFIX "c:\\tars\\cpp")
ENDIF()
set(CMAKE_INSTALL_PREFIX ${INSTALL_PREFIX})
endif()


#-------------------------------------------------------------
#set(APP_LIBRARIES)
set(OPEN_MYSQL "OFF")
set(OPEN_NGHTTP2 "OFF")
set(OPEN_SSL "OFF")
set(OPEN_ZLIB "OFF")
set(OPEN_PROTOBUF "OFF")

set(THIRDPARTY_PATH "${CMAKE_BINARY_DIR}/src")
if(TARS_MYSQL)
    set(OPEN_MYSQL "ON")
    set(MYSQL_DIR_INC "${THIRDPARTY_PATH}/mysql-lib/include")
    set(MYSQL_DIR_LIB "${THIRDPARTY_PATH}/mysql-lib/libmysql")
    include_directories(${MYSQL_DIR_INC})
    link_directories(${MYSQL_DIR_LIB})
endif()

if(TARS_PROTOBUF)
    set(OPEN_PROTOBUF "ON")
    set(PROTOBUF_DIR_INC "${THIRDPARTY_PATH}/protobuf-lib/src")
    set(PROTOBUF_DIR_LIB "${THIRDPARTY_PATH}/protobuf-lib")
    include_directories(${PROTOBUF_DIR_INC})
    link_directories(${PROTOBUF_DIR_LIB})
endif()

if(TARS_ZLIB)
    set(OPEN_ZLIB "ON")
    set(ZLIB_DIR_INC "${THIRDPARTY_PATH}/z-lib")
    set(ZLIB_DIR_LIB "${THIRDPARTY_PATH}/z-lib")
    include_directories(${ZLIB_DIR_INC})
    link_directories(${ZLIB_DIR_LIB})
endif()

if(TARS_HTTP2)
    set(OPEN_NGHTTP2 "ON")
    set(NGHTTP2_DIR_INC "${THIRDPARTY_PATH}/nghttp2-lib/lib/includes/")
    set(NGHTTP2_DIR_LIB "${THIRDPARTY_PATH}/nghttp2-lib/lib")
    include_directories(${NGHTTP2_DIR_INC})
    link_directories(${NGHTTP2_DIR_LIB})
endif()

if(TARS_SSL)
    set(OPEN_SSL "ON")
    set(SSL_DIR_INC "${THIRDPARTY_PATH}/openssl-lib/include/")
    set(SSL_DIR_LIB "${THIRDPARTY_PATH}/openssl-lib")
    include_directories(${SSL_DIR_INC})
    link_directories(${SSL_DIR_LIB})
endif()

#-------------------------------------------------------------

set(LIB_MYSQL)
set(LIB_NGHTTP2)
set(LIB_SSL)
set(LIB_CRYPTO)
set(LIB_ZLIB)
set(LIB_PROTOBUF)

IF (WIN32)
if(TARS_MYSQL)
    set(LIB_MYSQL "libmysql")
endif()    
if(TARS_HTTP2)
    set(LIB_NGHTTP2 "libnghttp2_static")
endif()    
if(TARS_SSL)
    set(LIB_SSL "libssl")
    set(LIB_CRYPTO "libcrypto")
endif()
if(TARS_ZLIB)
    set(LIB_ZLIB "libz")
endif()

if(TARS_PROTOBUF)
    set(LIB_PROTOBUF "libprotoc")
endif()    
ELSE()
    link_libraries(pthread dl)
if(TARS_MYSQL)
    set(LIB_MYSQL "mysqlclient")
endif()    
if(TARS_HTTP2)
    set(LIB_NGHTTP2 "nghttp2_static")
endif()

if(TARS_SSL)
    set(LIB_SSL "ssl")
    set(LIB_CRYPTO "crypto")
endif()    
if(TARS_ZLIB)
    set(LIB_ZLIB "z")
endif()    
if(TARS_PROTOBUF)
    set(LIB_PROTOBUF "protoc")
endif()    
ENDIF()


#-------------------------------------------------------------


#if(TARS_MYSQL)
#link_libraries(${LIB_MYSQL})
#endif()
#if(TARS_ZLIB)
#link_libraries(${LIB_ZLIB})
#endif()
#
#if(TARS_HTTP2)
#    list(APPEND APP_LIBRARIES ${LIB_PROTOBUF})
#endif()
#
#if(TARS_PROTOBUF)
#    list(APPEND APP_LIBRARIES ${LIB_NGHTTP2})
#endif()

if(TARS_SSL)
    link_libraries(${LIB_SSL} ${LIB_CRYPTO})
endif()

#list(LENGTH APP_LIBRARIES RET)
#if(${RET} GREATER 0)
#    link_libraries(${APP_LIBRARIES})
#endif()

include(ExternalProject)

if(TARS_PROTOBUF)
ExternalProject_Add(${LIB_PROTOBUF}
    URL http://cdn.tarsyun.com/src/protobuf-cpp-3.11.3.tar.gz
    PREFIX    ${CMAKE_BINARY_DIR}
    INSTALL_DIR ${CMAKE_SOURCE_DIR}
    CONFIGURE_COMMAND cmake cmake
    SOURCE_DIR ${CMAKE_BINARY_DIR}/src/protobuf-lib
    BUILD_IN_SOURCE 1
    BUILD_COMMAND make -j4 libprotoc
    LOG_CONFIGURE 1
    LOG_BUILD 1
    INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "install"
    URL_MD5 fb59398329002c98d4d92238324c4187
    )
endif()

if(TARS_MYSQL)
ExternalProject_Add(${LIB_MYSQL}
        URL http://cdn.tarsyun.com/src/mysql-5.6.26.tar.gz
        PREFIX    ${CMAKE_BINARY_DIR}
        INSTALL_DIR ${CMAKE_SOURCE_DIR}
        CONFIGURE_COMMAND cmake . -DDEFAULT_CHARSET=utf8 -DDEFAULT_COLLATION=utf8_general_ci -DDISABLE_SHARED=1
        SOURCE_DIR ${CMAKE_BINARY_DIR}/src/mysql-lib
        BUILD_IN_SOURCE 1
        BUILD_COMMAND make mysqlclient
        LOG_CONFIGURE 1
        LOG_BUILD 1
        INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "install"
        URL_MD5 c537c08c1276abc79d76e8e562bbcea5
        )
endif()

if(TARS_ZLIB)
ExternalProject_Add(${LIB_ZLIB}
        URL http://cdn.tarsyun.com/src/zlib-1.2.11.tar.gz
        PREFIX    ${CMAKE_BINARY_DIR}
        INSTALL_DIR ${CMAKE_SOURCE_DIR}
        CONFIGURE_COMMAND cmake .
        SOURCE_DIR ${CMAKE_BINARY_DIR}/src/z-lib
        BUILD_IN_SOURCE 1
        BUILD_COMMAND make zlibstatic
        LOG_CONFIGURE 1
        LOG_BUILD 1
        INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "install"
        URL_MD5 1c9f62f0778697a09d36121ead88e08e
        )
endif()

if(TARS_HTTP2)
ExternalProject_Add(${LIB_NGHTTP2}
    URL http://cdn.tarsyun.com/src/nghttp2-1.40.0.tar.gz
    PREFIX    ${CMAKE_BINARY_DIR}
    INSTALL_DIR ${CMAKE_SOURCE_DIR}
    CONFIGURE_COMMAND cmake . -DENABLE_SHARED_LIB=OFF -DENABLE_STATIC_LIB=ON -DENABLE_LIB_ONLY=ON
    SOURCE_DIR ${CMAKE_BINARY_DIR}/src/nghttp2-lib
    BUILD_IN_SOURCE 1
    LOG_BUILD 1
    LOG_CONFIGURE 1
    BUILD_COMMAND make
    INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "install"
    URL_MD5 5df375bbd532fcaa7cd4044b54b1188d
    )
endif()

if(TARS_SSL)
ExternalProject_Add(${LIB_SSL}
    DEPENDS ${LIB_ZLIB}
    URL http://cdn.tarsyun.com/src/openssl-1.1.1c.tar.gz
    PREFIX    ${CMAKE_BINARY_DIR}
    INSTALL_DIR ${CMAKE_SOURCE_DIR}
    CONFIGURE_COMMAND ./config
    SOURCE_DIR ${CMAKE_BINARY_DIR}/src/openssl-lib
    BUILD_IN_SOURCE 1
    BUILD_COMMAND make
    LOG_CONFIGURE 1
    LOG_BUILD 1
    INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "install"
    #URL_MD5 3be209000dbc7e1b95bcdf47980a3baa
    URL_MD5 15e21da6efe8aa0e0768ffd8cd37a5f6
    )
endif()

add_custom_target(thirdparty DEPENDS ${LIB_MYSQL} ${LIB_NGHTTP2} ${LIB_SSL})

#-------------------------------------------------------------
IF (APPLE)
link_libraries(iconv)
ENDIF(APPLE)

set(PLATFORM)
IF (UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11  -Wno-deprecated -fno-strict-aliasing -Wno-overloaded-virtual")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-builtin-macro-redefined -D__FILE__='\"$(notdir $(abspath $<))\"'")
    
    set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -Wall -g")
    set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall -fno-strict-aliasing")

    set(PLATFORM "linux")
    IF(APPLE)
        set(PLATFORM "mac")
        SET(CMAKE_C_ARCHIVE_CREATE   "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
        SET(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
        SET(CMAKE_C_ARCHIVE_FINISH   "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
        SET(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
    ENDIF(APPLE)

ELSEIF (WIN32)
    set(PLATFORM "window")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4101 /wd4244 /wd4996 /wd4091 /wd4503 /wd4819 /wd4200 /wd4800")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj " )

ELSE ()
    MESSAGE(STATUS "================ ERROR: This platform is unsupported!!! ================")
ENDIF (UNIX)

#-------------------------------------------------------------

IF(WIN32)
set(TARS2CPP "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/tars2cpp.exe")
ELSE()
set(TARS2CPP "${CMAKE_BINARY_DIR}/bin/tars2cpp")
ENDIF()

#-------------------------------------------------------------

IF(WIN32)
include_directories(${CMAKE_SOURCE_DIR}/util/src/epoll_windows)
ENDIF()

message("----------------------------------------------------")

message("CMAKE_BUILD_TYPE:          ${CMAKE_BUILD_TYPE}")
message("PLATFORM:                  ${PLATFORM}")
message("INSTALL_PREFIX:            ${INSTALL_PREFIX}")

#-------------------------------------------------------------

message("----------------------------------------------------")
message("OPEN_MYSQL:                ${OPEN_MYSQL}")
message("OPEN_NGHTTP2:              ${OPEN_NGHTTP2}")
message("OPEN_SSL:                  ${OPEN_SSL}")
message("OPEN_ZLIB:                 ${OPEN_ZLIB}")
message("OPEN_PROTOBUF:             ${OPEN_PROTOBUF}")

#-------------------------------------------------------------

include("${CMAKE_SOURCE_DIR}/cmake/BuildTarsServer.cmake")

include_directories(${CMAKE_SOURCE_DIR}/util/include)
include_directories(${CMAKE_SOURCE_DIR}/servant)
include_directories(${CMAKE_SOURCE_DIR}/servant/protocol)

add_subdirectory(util)
add_subdirectory(tools)
add_subdirectory(servant)
add_subdirectory(examples)

#add_subdirectory(test_deprecated)
