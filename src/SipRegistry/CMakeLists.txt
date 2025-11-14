cmake_minimum_required(VERSION 2.8)

project(VoipApp-SipRegistry)

option(TARS_MYSQL "option for mysql" ON)
option(TARS_SSL "option for ssl" OFF)
option(TARS_HTTP2 "option for http2" OFF)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(WIN32)
    include (c:\\tars\\cpp\\makefile\\tars-tools.cmake)
else()
    include (/usr/local/tars/cpp/makefile/tars-tools.cmake)
endif()

####you can: cd build; cmake .. -DTARS_WEB_HOST={your web host}
set(TARS_WEB_HOST "" CACHE STRING "set web host")
IF (TARS_WEB_HOST STREQUAL "")
	set(TARS_WEB_HOST "http://tars.test.com")
ENDIF ()

include_directories(/usr/local/tars/cpp/thirdparty/include)
# link_directories(/usr/local/tars/cpp/thirdparty/lib)

# 添加 TARS 源码头文件路径
include_directories(${CMAKE_SOURCE_DIR}/../../util/include)
include_directories(${CMAKE_SOURCE_DIR}/../../servant)
include_directories(${CMAKE_SOURCE_DIR}/../../servant/protocol)

add_subdirectory(src)

#target_link_libraries(mysqlclient ssl crypto nghttp2-static)
