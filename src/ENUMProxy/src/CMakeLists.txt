cmake_minimum_required(VERSION 2.8)

project(VoipApp-ENUMProxy)

# include directories (same layout as SipCallRelay)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../..)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../SipGateway/src)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../SipProxy/src)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../SipProxyDb/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../SipCallController/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../ENUMProxy/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../3rd/inc)

set(SRC_LIST
    ../src/ENUMProxy.cpp
    ../src/CallRoutingManager.cpp
    ../src/HttpClient.cpp
    ../src/HttpClientSession.cpp
    ../src/HttpServer.cpp
    ../src/HttpServerSession.cpp
    ../src/JsonUtil.cpp
    ../src/SipRouterImp.cpp
    ../../Common/SSLConnection.cpp
    ../../Common/Net.cpp
)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../3rd/lib/linux/x86_64)

gen_server(VoipApp ENUMProxy)

target_sources(ENUMProxy PRIVATE ${SRC_LIST})

target_link_libraries(ENUMProxy
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../build/lib/libtarsservant.a
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../build/lib/libtarsutil.a
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../3rd/lib/linux/x86_64/libjsoncpp.a
    -Wl,--no-whole-archive
    z
    dl
    pthread
)

#target_link_libraries(mysqlclient ssl crypto nghttp2-static)
