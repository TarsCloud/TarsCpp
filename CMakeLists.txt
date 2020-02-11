cmake_minimum_required(VERSION 2.8)

project(tars_cpp)
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

message("CMAKE_BUILD_TYPE TYPE = " ${CMAKE_BUILD_TYPE})

#编译的可执行程序输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(TARS_VERSION "2.0.0")
add_definitions(-DTARS_VERSION="${TARS_VERSION}")
set(TARS_SSL 1)
add_definitions(-DTARS_SSL=${TARS_SSL})
set(TARS_HTTP2 1)
add_definitions(-DTARS_HTTP2=${TARS_HTTP2})

set(_USE_OPENTRACKING $ENV{_USE_OPENTRACKING})
if(_USE_OPENTRACKING)
set(OPENTRACKING_INC "/usr/local/include")
add_definitions(-D_USE_OPENTRACKING=${_USE_OPENTRACKING})
endif()

#-------------------------------------------------------------
IF (UNIX)
set(INSTALL_PREFIX "/usr/local/tars/cpp")
ELSE()
set(INSTALL_PREFIX "c:\\tars\\cpp")
ENDIF()

set(CMAKE_INSTALL_PREFIX ${INSTALL_PREFIX})

#-------------------------------------------------------------

include(ExternalProject)

# ExternalProject_Add(thirdparty
#         # This is the upstream source code repackaged in a .tar.gz for
#         # compatibility with older CMake. Also the tests/ directory
#         # removed to save space.
#         URL http://cdn.tarsyun.com/src/ThirdParty.tgz
#         # URL /Users/jarod/centos/TarsCpp/build/thirdparty/src/20200208092837.tgz
#         PREFIX    ${CMAKE_BINARY_DIR}/thirdparty
#         INSTALL_DIR ${CMAKE_SOURCE_DIR}
#         #URL_MD5 ${clang_md5}
#         )


ExternalProject_Add(mysql-lib
    # URL ${CMAKE_SOURCE_DIR}/thirdparty-src/mysql-5.6.26.tar.gz
    URL http://cdn.tarsyun.com/src/mysql-5.6.26.tar.gz
    PREFIX    ${CMAKE_BINARY_DIR}
    INSTALL_DIR ${CMAKE_SOURCE_DIR}
    CONFIGURE_COMMAND cmake . -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/install/mysql-lib -DDEFAULT_CHARSET=utf8 -DDEFAULT_COLLATION=utf8_general_ci -DDISABLE_SHARED=1
    SOURCE_DIR ${CMAKE_BINARY_DIR}/src/mysql-lib
    BUILD_IN_SOURCE 1
    BUILD_COMMAND make -j4 mysqlclient
    LOG_CONFIGURE 1
    LOG_BUILD 1
    INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "install"
    URL_MD5 c537c08c1276abc79d76e8e562bbcea5
    )

ExternalProject_Add(nghttp2-lib
    URL http://cdn.tarsyun.com/src/nghttp2-1.40.0.tar.gz
    PREFIX    ${CMAKE_BINARY_DIR}
    INSTALL_DIR ${CMAKE_SOURCE_DIR}
    # CONFIGURE_COMMAND ./configure --enable-static --prefix=${CMAKE_BINARY_DIR}/install/nghttp2-lib
    CONFIGURE_COMMAND cmake . -DCMAKE_PREFIX_PATH=${CMAKE_BINARY_DIR}/install/nghttp2-lib -DENABLE_STATIC_LIB=1
    SOURCE_DIR ${CMAKE_BINARY_DIR}/src/nghttp2-lib
    BUILD_IN_SOURCE 1
    LOG_BUILD 1
    LOG_CONFIGURE 1
    BUILD_COMMAND make -j4
    INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "install"
    URL_MD5 5df375bbd532fcaa7cd4044b54b1188d
    )

ExternalProject_Add(openssl-lib
    URL http://cdn.tarsyun.com/src/openssl-1.1.1d.tar.gz
    PREFIX    ${CMAKE_BINARY_DIR}
    INSTALL_DIR ${CMAKE_SOURCE_DIR}
    CONFIGURE_COMMAND ./config --prefix=${CMAKE_BINARY_DIR}/install/openssl-lib
    SOURCE_DIR ${CMAKE_BINARY_DIR}/src/openssl-lib
    BUILD_IN_SOURCE 1
    BUILD_COMMAND make -j4
    LOG_CONFIGURE 1
    LOG_BUILD 1
    INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "install"
    URL_MD5 3be209000dbc7e1b95bcdf47980a3baa
    )

add_custom_target(thirdparty DEPENDS nghttp2-lib openssl-lib mysql-lib)

set(THIRDPARTY_PATH "${CMAKE_BINARY_DIR}/src")

set(MYSQL_DIR_INC "${THIRDPARTY_PATH}/mysql-lib/include")
set(MYSQL_DIR_LIB "${THIRDPARTY_PATH}/mysql-lib/libmysql")
include_directories(${MYSQL_DIR_INC})
link_directories(${MYSQL_DIR_LIB})

set(NGHTTP2_DIR_INC "${THIRDPARTY_PATH}/nghttp2-lib/lib/includes/")
set(NGHTTP2_DIR_LIB "${THIRDPARTY_PATH}/nghttp2-lib/lib")
include_directories(${NGHTTP2_DIR_INC})
link_directories(${NGHTTP2_DIR_LIB})

set(SSL_DIR_INC "${THIRDPARTY_PATH}/openssl-lib/include/")
set(SSL_DIR_LIB "${THIRDPARTY_PATH}/openssl-lib")
include_directories(${SSL_DIR_INC})
link_directories(${SSL_DIR_LIB})

set(LIB_MYSQL)
set(LIB_NGHTTP2)
set(LIB_SSL)
set(LIB_CRYPTO)

IF (WIN32)
    set(LIB_MYSQL "libmysql")
    set(LIB_NGHTTP2 "libnghttp2_static")
    set(LIB_SSL "libssl")
    set(LIB_CRYPTO "libcrypto")
ELSE()
    link_libraries(pthread dl)
    set(LIB_MYSQL "mysqlclient")
    set(LIB_NGHTTP2 "nghttp2_static")
    set(LIB_SSL "ssl")
    set(LIB_CRYPTO "crypto")
ENDIF()

link_libraries(${LIB_MYSQL})

if(TARS_HTTP2)
link_libraries(${LIB_NGHTTP2})
endif()

if(TARS_SSL)
link_libraries(${LIB_SSL})
link_libraries(${LIB_CRYPTO})
endif()

#-------------------------------------------------------------
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
    ENDIF(APPLE)

ELSEIF (WIN32)
    message("window.....")
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

include("${CMAKE_SOURCE_DIR}/cmake/BuildTarsServer.cmake")

include_directories(${CMAKE_SOURCE_DIR}/util/include)
include_directories(${CMAKE_SOURCE_DIR}/servant)
include_directories(${CMAKE_SOURCE_DIR}/servant/protocol)

add_subdirectory(util)
add_subdirectory(tools)
add_subdirectory(servant)
add_subdirectory(examples)

#add_subdirectory(test_deprecated)
