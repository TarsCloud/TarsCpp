
# v3.0.14 20230307
### en
- Fix: tars2node to avoid warning
- Fix: possible memory leaks
- Feat: log add prefix
- Feat: add tc_split
- Feat: refreshReg when keep alive

### cn
- 修复: tars2node to avoid warning
- 修复: 可能的内存泄漏
- 特性: 日志增加前缀
- 特性: 添加tc_split
- 特性: keepAlive的时候refreshReg

# v3.0.13 20221117
### en
- Fix: TC_Port::getSystemMemInfo not close fd bug
- Fix: threadpool waitForAllDone not quit bug in some time
- Fix: tc_common str2tm add memset tm
- Fix: QueueMode bug when fd=-1
- Fix: tarsmerge bug when multi tars files, orderFileNames.erase(it->second) cause crash
- Fixed the HTTP client crash when compiling with SSL and the unusable HTTP client when compiling without SSL
- Fix: [MSVC][std:c++latest] Fix compiler error
- Feat: tc_http dorequest support https
- Feat: ServantHandle&async_callback支持右值引用参数
- Feat: PushCallbackDemo

### cn
- 修复: TC_Port::getSystemMemInfo异常时没有关闭句柄的bug
- 修复: 线程池 waitForAllDone 有时候不退出的bug
- 修复: tc_common str2tm 初始化tm(memset), 会导致时间获取不准
- 修复: 服务QueueMode模式下fd=-1导致进错队列的bug
- 修复: tars2merge 多层嵌套tars文件，当第二次移除同一元素时会崩溃 orderFileNames.erase(it->second);
- 修复: [MSVC][std:c++latest] 修复编译错误
- 修复: tc http在https模式下http doRequest的crash
- 新增: 新增 PushCallbackDemo
- 新增: tc_http dorequest 支持 https
- 新增: ServantHandle&async_callback支持右值引用参数

# v3.0.12 20220815

### en

- Fix: TC_Shm bug, can not delete shm after detach
- Fix: http_call memory leak
- Fix: tars2node list & map as interface paramters bug when http+json call bug
- Fix: epoll server doResponse close connection when client close connection
- Fix: getConnStatus bug that return all connections on all listen ports
- Fix: the problem that no root servantproxy will not be called in the scenario where multi thread calls are started at the same time when serial calls are made(http mode).
- Fix: Asynchronous callback if dyeing open, active enabledying is required
- Feat: tars2node add tars_ping
- Feat: add common_protocol_call, Support any short connection mode similar to http
- Feat: Add server stat(Current)
- Feat: Reconnect supports millisecond intervals, and supports reconnection only for those in active state in the master registration list, to avoid invalid operations after capacity reduction on k8s

 
### cn
- Fix: TC_Shm 修复无法删除shm的bug
- Fix: http_call内存泄露的bug
- Fix: tars2node修复当list & map作为接口参数时, 无法通过http+json调用的bug
- Fix: epoll server 根据 doResponse参数来主动关闭连接的问题, 避免管理连接不及时
- Fix: getConnStatus 修复返回了所有监听端口的连接的bug
- Fix: 修复串行调用的时候，同时启动多线程调用的场景，非根servantproxy对应的prx不会被调用的问题
- Fix: 异步回调如果判断如果染色了需要主动enableDyeing
- Feat: tars2node 添加 tars_ping
- Feat: ServantProxy添加 common_protocol_call, 支持任何类似http的短连接模式
- Feat: 增加服务端请求统计(Current)
- Feat: reconnect支持毫秒间隔，并且支持只对在主控注册列表里为active状态的进行重连，避免k8s上缩容后无效操作

# v3.0.11 20220628

### en

- Add: tc_port add some func(now only work in linux)
- Add: tc_file listDirectory add ignoreHide
- Add: tc_common add UTC2LocalTime
- Fix: tc_clientsocket unix local socket bug
- Fix: tars2cpp sendResponse use TarsOutputStream
### cn
- Add: tc_port 添加了一些linux下的函数, 获取服务的启动时间
- Add: tc_file listDirectory 添加 ignoreHide参数
- Add: tc_common 添加 UTC2LocalTime函数
- Fix: tc_clientsocket unix local socket bug
- Fix: tars2cpp 回包时使用 TarsOutputStream, 减少一次内存copy

# v3.0.10 20220606

### en

- Add: tc_clientsocket support unix local domain socket
- Add: tc_http support unix local domain socket, Note that '/' used instead of '$' in the URL. If the port is 0, it is a local socket
- Add: tc_docker use TC_Http::doRequest

### cn

- Add: tc_clientsocket 支持 unix 本地套接字
- Add: tc_http 支持本地套接字, 注意 url 中/使用$代替, 端口为 0 的情况下为本地套接字
- Add: tc_docker 使用 TC_Http::doRequest

# v3.0.9 20220530

### en

- Fix: update gtest to fix compiler bug in high gcc
- Fix: makefile.tars support .cc .c
- Fix: TC_NetWorkBuffer bug, Optimize doProtocolAnalysis & getBuffers bug cause crash
- Fix tc_logger thread use unique_ptr
- Optimize: rpc tars_hash use uint32_t and tars2cpp tars_hash use uint32_t
- Optimize: move AppProtocol func imp to cpp
- Add: tc_docker add exec
- Add: tc_config add erase/eraseDomain
- Add: servant add doNoFunc
- Add: add tars interface push support

### en

- 修复: 更新 gtest 在高版本 gcc 下编译的 bug
- 修复: 修改 makefile.tars 支持 .cc .c
- 修复: TC_NetWorkBuffer 优化 doProtocolAnalysis 逻辑以及 getBuffers 导致的 crash(windows 下)
- 修复: tc_logger 线程指针使用 unique_ptr
- 优化: rpc tars_hash 使用 uint32_t 以及 tars2cpp 调用 tars_hash 使用 uint32_t
- 优化: AppProtocol 增加实现文件
- 添加: tc_docker 添加 exec 函数
- 添加: tc_config 添加 erase/eraseDomain
- 添加: 服务端 servant 添加 doNoFunc
- 添加: tars 接口支持 push 接口

# v3.0.8 20220429

### en

- Fix : EndpointManager updateOutter cause crash bug
- Fix: notify & report add ServerConfig::Context(node_name)
- Fix: ObjectProxy initialize & doTimeout no in same thread, may cause crash
- Add: tc_json JsonValueArray add `find` func
- Optimize: compiler not install openssl docs
- Optimize: thread process may delay in epoll_server NET_THREAD_MERGE_HANDLES_THREAD mode
- Add: TC_ThreadPoolHash::getThread
- Add: LOG_CONSOLE_DEBUG add millsecond time output
- Fix: tc_http strnstr bug cause crash

### cn

- 修复: 修复 EndpointManager updateOutter 并发导致的 crash
- 修复: tarscpp 框架中, notify & report 时添加 ServerConfig::Context(node_name)
- 修复: ObjectProxy 初始化和 doTimeout 不在同一个线程中可能导致的 crash
- Add: tc_json JsonValueArray 添加 `find` 函数
- 优化: 编译 openssl 时, 不安装 docy
- 优化: NET_THREAD_MERGE_HANDLES_THREAD 模式下可能的延时问题
- 添加: TC_ThreadPoolHash::getThread
- 添加: LOG_CONSOLE_DEBUG 调整为毫秒输出, 方便调试
- 修复: tc_http strnstr 的 bug 导致的 crash

# v3.0.7 20220328

### en

- Add: tc_json adds the function of merge
- Fix: tc_endpoint sepEndpoint bug
- Fix: there is no business interface callback if there is no available node during asynchronous call
- Optimize: In the service model, after receiving data, it is inserted into the queue in batch
- Optimize: the time occupation when parsing the protocol at the network layer, and release the network thread every 1ms to reduce the delay

### cn

- 添加: tc_json 添加 merge 函数
- 修复: tc_endpoint sepEndpoint bug
- 修复: 异步调用模式下, 如果没有活跃的节点, 导致回调丢失的问题
- 优化: 在服务模型中, 接收数据后, 批量插入队列
- 优化: 网络层延时控制在 1ms 就释放, 降低服务的延时

# v3.0.6 20220321

### en

- Add: test_tc_file add config unittest
- Add: add tc_docker, docker api for tarsnode/tarsregistry use
- Add: tc_http_async support unix socket(not work in windows)
- Add: tc_http parsing supports scenarios where content-length does not exist
- Fix: Tars.h compiler warning
- Fix: single timer work cause memory leak in tc_timer
- Fix: tc_socket bind, throw TC_Socket_Exception with ip port
- Fix: tc_logger MAX_BUFFER_LENGTH -> 1M

### cn

- 添加: test_tc_file 添加了一个配置的测试用例
- 添加: 添加 tc_docker, 聚合和 docker api, 主要给 tarsnode/tarsregistry 使用
- 添加: tc_http_async 支持了 unix socket(windows 下不工作)
- 添加: tc_http 解析支持了 http 头中 content-length 不存在的情况
- 修复: Tars.h 修复了一个编译警告
- 修复: tc_timer 单任务仍然会导致内存泄露的问题
- 修复: tc_socket 调用 bind 的时, 抛出 TC_Socket_Exception 并附带了 ip port
- 修复: tc_logger MAX_BUFFER_LENGTH 调整为 1M

# v3.0.5 20220301

### en

- support tarscpp server run container
- Fix memory leak caused by timer
- Fix crash caused by transceiver in case of connection timeout
- Fix crash caused by epoller

### cn

- 支持框架容器化运行
- 修复定时器带来的内存泄露
- 修复 tc_transceiver 在连接超时情况下可能带来的 crash
- 修复 tc_epoller 可能带来的 crash

# v3.0.4 20220118

### en

- prx supports automatic heartbeat (it needs to be configured and enabled)
- prx supports multiple connections in the form of hash tag((test characteristics)
- Add consistency hash unit test case
- Solve the consistency hash exception during dynamic expansion and contraction of k8s
- prx adds tars_open_trace, actively open the call chain
- In the HTTP mode, prx automatically sets the host when the host is not set

### cn

- prx 支持自动心跳(需要配置开启)
- prx 以 hash tag 的方式支持多连接(试验特性)
- 添加一致性 hash 单元测试用例
- 解决 k8s 等动态扩缩容时一致性 hash 异常
- prx 增加 tars_open_trace, 主动开启调用链
- prx 在 http 模式下, 当 host 没有设置的情况下, 自动设置 host

# v3.0.3 20211125

### en

- TarsJson support map<TarsStructBase ....> format
- tc_epoller remove assert(info->\_epoller == this), that will cause coredump(reason not known)
- fix FFDLOG not work bug
- fix \_traceCall not initialize bug
- tars2cpp add tars::TC_Common::equal when field is float or double
- tars json support enum as map key

### cn

- 修复 TarsJson 支持 Tars 结构体作为 Key
- 去掉 tc_epoller 中的 assert 判断, 会导致服务 core, 原因未知(该问题还需持续跟中)
- 修复 FDDLOG 不工作的问题
- 修复\_traceCall 没有初始化的问题, 会导致独立的客户端启用调用链抛异常
- 修复 tars2cpp 生成代码对 double/float 直接用=判断的问题
- TarsJon 支持枚举类型作为 key

-

# v3.0.2 20211115

### en

- reset inner struct/vector/map when resetDefautlt
- Fix the problem that the core will be when the independent communicator client Ctrl + C exits
- fix TC_Coroutine rpc call not open co
- fix co examples bug
- fix service model, the connection obtained is TC_Transceiver, and fix this problem with TC_Transceiver is changed to Connection to maintain consistency with version 2. X
- fix unittest compiler bug, need open cmath
- fix tc_base64 crash bug
- fix build error on windows
- fix: fix tc_autoptr.h Type p to nullptr

### cn

- 修复 struct/vector/map, 支持 resetDefautlt
- 修复独立通信器的客户端 ctrl+C 退出时会 crash 的问题
- 修复 TC_Coroutine rpc 调用时没有开启 co 的支持
- 修复协程例子的 bug
- 修复服务模型中, 获取的 Connection 是 TC_Transceiver 的问题, 改成了 Connection, 保持和 2.x 版本一致性
- 修复 unittest 编译的 bug, 缺少了 cmath 头文件
- 修复 tc_base64 的在某些字符下 decode 的 bug
- 修复 windows 下的编译 bug
- json 输出支持不包含空格的紧凑格式

# v3.0.1 20211017

### en

- Built in support call chain
- Fix some file utf8bom encoding and compilation errors under windows
- Fix When the complex epoll server only pushes data, it does not update the link time, which leads to the problem that the link is closed
- tars-tools.cmake adds variable settings to support k8s Web
- Bug in tars protocol parsing when the repair package size is 20 bytes
- Increase promise support

### cn

- 内置支持调用链
- 修复部分文件 utf8bom 编码, 以及 windows 下编译的错误
- 修复 epollserver 在只推送数据的情况下, 没有更新链接时间导致链接被关闭的问题
- tars-tools.cmake 增加变量设置, 支持 k8s web
- 修复包大小 20 字节情况下, tars 协议解析出现的 bug
- 增加 promise 的支持

# v3.0.0 20210911

### en

- submit cor

### cn

- 协程版本代码提交
