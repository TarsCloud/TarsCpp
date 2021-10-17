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