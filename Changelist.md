## v2.4.3 2020.06.10
### en:
- tc_spin_lock support arm
- fix tc_http security problem
- fix tc_mysql table name with database name bug
- tarscpp create tars protocol, default package optional field
- fix tars2case 的bug

### cn:
- tc_spin_lock 支持arm
- tc_http解析安全问题
- tc_mysql 表名带db名称时 bug的问题
- tarscpp协议缺省值默认打包
- fix tars2case 的bug

## v2.4.2 2020.06.02
### en:
- Add English commet to some header file commet
- tars2case support json format
- fix tc_http bug, ignore last header line
- fix tc_json null bug and add interface
### cn:
- 部分头文件注释增加英文注释
- tars2case支持json格式
- 修复tc_http解析头, 漏掉最后一行的bug
- 修复tc_json null的bug 以及修改tc_json的函数接口, 更易用


## v2.4.1 2020.05.06
### en:
1 fix windows tars proto, define long bug, change to long long
2 fix windows, longlong tars_ntohll bug 
3 fix windows, tc_ex system message pointer may be null cause crash 
4 fix windows tc_epoll_server when send pending add epoll mod
### cn:
1 修复windows 64位 tars协议中, long 定义的问题(为32位,改成long long)
2 修复windows 64位整形, 字节序转换的问题
3 修复windows tc_ex中系统消息指针为NULL的bug
4 修复windows windows tc_epoll_server when send pending add epoll mod

## v2.4.0 2020.04.24
### en:
- fix tars2json bug of array
- TC_EpollServer::Connction add _authInit initialize
- fix tarsnotify nodename bug
- compiler & install framework/*.tars in tarscpp
- fix tars-tools.cmake 'make tar' bug in linux
- In epoll server, change tc_cas_queue to tc_thread_queue
- tc_thread queue add front & pop_front
- add tars to xml support , add tc_xml.h/cpp tool class
- fix tc_json bug in float/double
- tars2cpp add interface to get/set sql
- TarsOutpuStream member change to public
- add tars2case for benchmark

### cn:
- 修复tars2json中 array的bug
- TC_EpollServer::Connction 构造时，增加对_authInit变量初始化
- 修复tarsnotify nodename bug
- compiler & install framework/*.tars in tarscpp
- 修复tars-tools.cmake make tar在linux的bug
- epoll server中tc_cas_queue改为 tc_thread_queue, tc_cas_queue在极端压力下可能会有问题, 原因待查
- tc_thread queue add front & pop_front
- 增加tars to xml的支持, 增加tc_xml.h/cpp解析类
- 修复tc_json的float/double的bug
- tars2cpp 工具增加将结构体转换成sql的接口
- TarsOutpuStream成员变量改成public, 便于外部访问
- 增加tars2case工具, 给benchmark工具使用

## v2.3.0 2020.04.02
### en:
- modify cookies
- To support short connection (HTTP), servantproxy adds connection serial mode
- Support http1 client
- The interface of http1 and http2 is unified (http2 is not compatible before, and there are bugs in the previous design)
- Optimize HTTP performance
- Modify the file name and some class names at the beginning of tars to facilitate code merging
- Improve the logic of service IP list switching
### cn:
- cookies修改实现方式
- 为支持短连接(http), ServantProxy增加连接串行模式
- rpc支持http1客户端
- 统一了http1 和 http2的接口(http2不在兼容之前的, 之前的设计上是有bug的)
优化http性能
- 修改Tars开头的文件名以及部分类名, 方便代码合并
- 改进服务ip list切换的逻辑

## v2.2.0 2020.04.02
## en
- fix epoll bug in windows
## cn
- 修复epoll在windows下的bug

## v2.1.2 2020.03.28
### en:
- Fix bugs in tar-tools.cmake's support for make release
- cmake_tars_server.sh Permissions are automatically set to executable during installation
- fix tc_common::strto bug in unsigned char
- Download and compile curl automatically under windows to support the subsequent make upload
- Repair the support of tars-tools.cmake for make upload under Windows
### cn:
- 修复tars-tools.cmake对release支持的bug
- cmake_tars_server.sh等安装时权限都自动设置为可执行
- tc_common::strto修复unsigned char的bug
- windows下自动下载编译curl,为后续make upload提供支持
- 修复windows下tars-tools.cmake对make upload的支持

## v2.1.1 2020.03.23
### en:
- update tars2node, support ts
- tc_json add get
- fix CMakeLists.txt in servant demo
### cn:
- 更新tars2node, 完善对ts的支持
- tc_json增加一个api
- 修改了脚本创建服务后, CMakeLists.txt模板, 默认管理ssl http2

## v2.1.0 2020.03.14
### en:
- udp in ipv6 bug
- add tars.resource
- fix rsp queue
- add max buffer limit, send queue, data accumulate clear
- Adjust cmake third library path
- tc_network_buffer add iterator
- optimize tc_http tc_http_async, improve http parser performance
- fix tars2node, support --with-tars
- remove rapidjson depends
### cn:
- 修改udp在ipv6下的bug
- 服务框架增加tars.resource命令字, 可以查看服务内网络连接细节
- 修改响应队列上报的bug
- 增加发送buffer过载保护
- 调整第三方库安装后的库路径
- tc_network_buffer增加iterator接口
- 优化tc_http tc_http tc_http_async性能, 提升http parser
f- ix tars2node, support --with-tars

## v2.0.0 2020.03.03
- support linux mac windows
- support ARM
- Reduce memory copy and improve performance
- Download compile dependencies automatically
- fix epoll server dead cycle bug
- Improve compilation mode and rely on MySQL client Lib
- fix some bug