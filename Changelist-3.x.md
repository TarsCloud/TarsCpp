
# v3.0.7 20220328

### en

- Add: tc_json adds the function of merge
- Fix: tc_endpoint sepEndpoint bug
- Fix:  there is no business interface callback if there is no available node during asynchronous call
- Optimize: In the service model, after receiving data, it is inserted into the queue in batch
- Optimize: the time occupation when parsing the protocol at the network layer, and release the network thread every 1ms to reduce the delay

### cn

- 添加: tc_json 添加merge函数
- 修复: tc_endpoint sepEndpoint bug
- 修复: 异步调用模式下, 如果没有活跃的节点, 导致回调丢失的问题
- 优化: 在服务模型中, 接收数据后, 批量插入队列
- 优化: 网络层延时控制在1ms就释放, 降低服务的延时

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
- 添加: 添加 tc_docker, 聚合和docker api, 主要给tarsnode/tarsregistry使用
- 添加: tc_http_async 支持了 unix socket(windows下不工作)
- 添加: tc_http 解析支持了http头中content-length不存在的情况
- 修复: Tars.h 修复了一个编译警告
- 修复: tc_timer单任务仍然会导致内存泄露的问题
- 修复: tc_socket调用bind的时, 抛出 TC_Socket_Exception 并附带了ip port
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
