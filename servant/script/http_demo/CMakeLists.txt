cmake_minimum_required(VERSION 2.8)

project(Demo-DemoServer)

option(TARS_MYSQL "option for mysql" ON)
option(TARS_SSL "option for ssl" ON)
option(TARS_HTTP2 "option for http2" ON)

if(WIN32)
    include (c:\\tars\\cpp\\makefile\\tars-tools.cmake)
else()
    include (/usr/local/tars/cpp/makefile/tars-tools.cmake)
endif()

set(TARS_WEB_SERVER "upchina@taf.test.whup.com")
set(TARS_WEB_HOST "taf.test.whup.com:8080")

include_directories(/usr/local/taf/cpp/thirdparty/include)
link_directories(/usr/local/taf/cpp/thirdparty/lib)

#include_directories(/home/tarsprotol/App/OtherServer)

gen_server(Demo DemoServer)
