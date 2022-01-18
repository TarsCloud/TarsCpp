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
