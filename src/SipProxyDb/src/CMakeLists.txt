cmake_minimum_required(VERSION 2.8)

project(VoipApp-SipProxyDb)

# 使用旧的 C++ ABI 以兼容 3rd 目录中的预编译库（如 libjsoncpp.a）
#add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

# 添加头文件搜索路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../..)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
# 添加各子模块的 src 目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../DbConn)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../3rd/inc)

# 添加依赖的源文件（不再包含 DbConn 的源文件，改为链接库）
set(SRC_LIST
    ../src/SipProxyDb.cpp
    ../src/SipDbImp.cpp
    ../../DbConn/MysqlHelper.cpp  # MysqlHelper 可能不在库中，保留
)

# 链接库路径
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../3rd/lib/linux/x86_64)

gen_server(VoipApp SipProxyDb)

# 链接依赖的源文件
target_sources(SipProxyDb PRIVATE ${SRC_LIST})

# 链接库和其依赖的库
# 注意：链接顺序很重要，被依赖的库要放在后面
# 使用本地编译好的 TARS 库（需要先在根目录编译一次 TARS）
target_link_libraries(SipProxyDb 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../build/lib/libtarsservant.a
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../build/lib/libtarsutil.a
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../3rd/lib/linux/x86_64/libjsoncpp.a
    -Wl,--no-whole-archive
    mysqlclient
    ssl
    crypto
    z
    dl
    pthread
)
