cmake_minimum_required(VERSION 2.8)

project(VoipApp-SipCallRelay)

# 使用旧的 C++ ABI 以兼容 3rd 目录中的预编译库（如 libjsoncpp.a）
#add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

# 添加头文件搜索路径
# 使用 CMAKE_CURRENT_SOURCE_DIR 确保路径正确
# 添加 src 目录作为根目录，使得可以通过 SipGateway/src/xxx.h 这样的路径来引用头文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../..)
# 添加各子模块的 src 目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../SipGateway/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../SipProxy/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../SipProxyDb/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../SipCallController/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../ENUMProxy/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Common)
# 添加 avatar 库的头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../3rd/inc)

# 添加依赖的源文件
#set(SIPGATEWAY_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../SipGateway/src)
#set(SIPPROXY_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../SipProxy/src)

# 添加 SipGateway 相关源文件
#aux_source_directory(${SIPGATEWAY_SRC_DIR} SIPGATEWAY_SRCS)

# 添加 SipProxy 相关源文件  
#aux_source_directory(${SIPPROXY_SRC_DIR} SIPPROXY_SRCS)

set(SRC_LIST
    ../src/SipCallRelay.cpp
    ../src/SipRelayImp.cpp
    ../../SipCallController/src/SipDialog2.cpp
    ../../SipGateway/src/ConnectionCallIdManager.cpp
    ../../SipGateway/src/SipDialog.cpp
    ../../SipGateway/src/SipGatewayConnection.cpp
    ../../SipGateway/src/SipMsg.cpp
    ../../SipGateway/src/SipMsgCommon.cpp
    ../../SipGateway/src/SipMsgDecode.cpp
    ../../SipGateway/src/SipMsgDecodedPart.cpp
    ../../SipGateway/src/SipMsgEncode.cpp
    ../../SipGateway/src/tinyxml2.cpp
    ../../SipProxy/src/SipGatewayIdRecordManager.cpp
    ../../SipProxy/src/SipRegisterSubscribe.cpp
)

# 链接 avatar 库路径
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../3rd/lib/linux/x86_64)

gen_server(VoipApp SipCallRelay)

# 链接依赖的源文件
target_sources(SipCallRelay PRIVATE ${SRC_LIST})

# 链接 avatar 库和其依赖的库
# 注意：链接顺序很重要，被依赖的库要放在后面
# 使用本地编译好的 TARS 库（需要先在根目录编译一次 TARS）
target_link_libraries(SipCallRelay 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../build/lib/libtarsservant.a
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../build/lib/libtarsutil.a
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../3rd/lib/linux/x86_64/libavatar.a
    -Wl,--whole-archive
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../3rd/lib/linux/x86_64/libjsoncpp.a
    -Wl,--no-whole-archive
    z 
    dl
    pthread
)

#target_link_libraries(mysqlclient ssl crypto nghttp2-static)
