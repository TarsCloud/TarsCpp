cmake_minimum_required(VERSION 2.8)

project(VoipApp-SipRegistry)

# 添加头文件搜索路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../..)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 源文件列表
set(SRC_LIST
    SipRegistry.cpp
    SipRegImp.cpp
)

# 生成服务
gen_server(VoipApp SipRegistry)

# 添加源文件
target_sources(SipRegistry PRIVATE ${SRC_LIST})

# 链接 TARS 库
# 使用本地编译好的 TARS 库（需要先在根目录编译一次 TARS）
target_link_libraries(SipRegistry 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../build/lib/libtarsservant.a
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../build/lib/libtarsutil.a
    pthread
    dl
)

#target_link_libraries(mysqlclient ssl crypto nghttp2-static)
