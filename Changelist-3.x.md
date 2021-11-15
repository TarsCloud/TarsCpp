
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
- 修复struct/vector/map, 支持resetDefautlt
- 修复独立通信器的客户端ctrl+C退出时会crash的问题
- 修复TC_Coroutine rpc调用时没有开启co的支持
- 修复协程例子的bug
- 修复服务模型中, 获取的Connection是TC_Transceiver的问题, 改成了Connection, 保持和2.x版本一致性
- 修复unittest编译的bug, 缺少了cmath头文件
- 修复tc_base64的在某些字符下decode的bug
- 修复windows下的编译bug
- json输出支持不包含空格的紧凑格式


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
- 修复部分文件utf8bom编码, 以及windows下编译的错误
- 修复epollserver在只推送数据的情况下, 没有更新链接时间导致链接被关闭的问题
- tars-tools.cmake 增加变量设置, 支持k8s web
- 修复包大小20字节情况下, tars协议解析出现的bug
- 增加promise的支持

# v3.0.0 20210911
### en
- submit cor
### cn
- 协程版本代码提交