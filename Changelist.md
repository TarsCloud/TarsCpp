# v2.4.24 2021.11.25

### en

- tars2cpp support double/float use TC_Common::equal
- tars struct add map/clear resetDefault
- tc_json support enum & tars struct as map key
- fix FFDLOG bug

### cn

- tars2cpp 支持 double/float 的比较, 使用 TC_Common::equal
- tars2cpp 支持 tars 结构体中有 map/clear 的 resetDefault
- tc_json 支持枚举以及 tars 结构体作为 map 的 key
- 修复 FFDLOG 的 bug

# v2.4.23 2021.11.16

### en

- Fix tc_base64 bg, the code bug caused by =, which should be supplemented at the end, but there is no supplement
- Fix windows encode utf8bom

### cn

- 修复 tc_base64 bug, 结尾有=填充, 但是又没有的情况
- 修复 windows 版本编译的 bug, 变成 utf8bom 格式

# v2.4.22 2021.10.08

### en

- There can be multiple circular logs, and tlog can be used directly TLOG_EX to add multiple circular logs
- Fix the bug in the parsing protocol when the package is less than 20 bytes

### cn

- 循环日志可以多个, 可以直接使用 TLOG_EX 来增加多个循环日志
- 修复当包体小于 20 个字节时, 解析协议出现的 bug

# v2.4.21 2021.08.19

### en

- fix StatReport crash when communicatorEpoll delete
- update protocol to support K8S
- tars-tools.cmake support K8S upload
- tc_socket bind output to cerr when failed
- fix tc_json double bug

### cn

- 修复 StatReport 可能的 crash 当 communicatorEpoll 被 delete 时
- 为了后续支持 K8S 更新了相关的协议
- 更新了 tars-tools.cmake support K8S upload
- tc_socket 增加了 cerr 的输出, 帮忙定位问题
- 修复 tc_json double 精度的 bug

# v2.4.20 2021.07.27

### en

- application add addServantWithParams support parameter for handle
- remove message.h opentracking define
- fix tc_timer memory leak and timer id duplicate bug
- support grpc and add GrpcDemo and fix tc_grpc compiler
- 修复 windows 下 socket 设置阻塞异常
- 修复 refresh-endpoint-interval 配置无效
- update unittest

### cn

- Application 添加 addServantWithParams, 支持 Servant 构造参数
- 去掉 Message.h opentracking 定义
- 修复 tc_timer 内存泄露和 timer id 可能冲突的问题
- 支持 grpc GrpcDemo, 修复 tc_grpc compiler
- 修复 windows 下 socket 设置阻塞异常
- 修复 refresh-endpoint-interval 配置无效
- 更新 unittest

# v2.4.19 2021.05.18

### en

- fix TARS_OPENTARCKING not report trace bug
- update mysql-client to support cmake 3.20
- fix doc README.md 404

### cn

- 修复打开 TARS_OPENTARCKING 宏时，没有上报 trace 信息的问题
- update mysql-client to support cmake 3.20
- fix doc README.md 404

## v2.4.18 2021.04.13

### en

- fix tc_thread_queue,Optimizing the swarm effect
- fix epoll server connection close cause crash bug

### cn

- 修复 tc_thread_queue 优化惊群效应
- 修复 epoll server 连接关闭时可能带来的 crash

# v2.4.17 2021.04.02

### en

- fix tc_time bug of erase caused by timer repeat event
- Fix the MySQL client library to solve the problem of compiling under cmake3.20
- Fix tc_config, key has no escaped bug
- Fix the scheduling mechanism to avoid the problem of 1 s scheduling delay

### cn

- 修复 tc_timer repeat 事件带来的无法 erase 的问题
- 修复引用的 mysql-client 库, 解决 cmake3.20 下编译的问题
- 修复 tc_config key 没有转义的 bug
- 修复协程调度机制, 避免出现总是出现 1s 调度延时的问题

# v2.4.16 2021.03.28

### en

- tc_epoll_server add sendBufferDirect
- tc_epoll_server fd auto closed when forked
- fix tc_common time zone bug
- fix hash call bug
- fix EndpoingManager.cpp add missing break
- Fix bug that Popen fails to return null pointer and uses it directly without judgment.
- fix to avoid coredump while application exit
- fix hash table and consistent hash ring are not updated when service node weight is updated
- fix crash When the number of processing pressure test connections is greater than the maximum number of service connections, and the connections are established and disconnected frequently
- fix In Linux and windows, it is a small end. It corrects the definition error of byte order conversion function, such as tars\_ ntonll
- set timeout options to prevent "connnect", "real_query" to be blocked for tens of minutes
- Fix the function name (tc_bitmap) in the exception
- update protocol

### cn

- tc_epoll_server 添加 sendBufferDirect
- tc_epoll_server fd 添加 fork 后自动 close 属性
- 修复 tc_common 中时区函数的时区的 bug
- 修复 hash 第一次建立连接不稳定的 bug
- 修复 EndpoingManager.cpp 缺少 missing break 的 bug
- 修复服务退出时可能的 crash
- 修复 popen 失败返回空指针 没有判断就直接使用的 BUG。
- 修复解决服务节点权重更新时哈希表和一致性哈希环不更新的问题
- 修复同一服务有多个 obj 的情况，同一 hash 值调用不同的 obj 会 hash 到不同的服务器，因为 addNode 会根据 desc(ip+port)计算 md5,导致顺序不一致
- 修复处理压测连接数大于服务最大连接数,并且有连接频繁建立、断开时
- 修复 windows 下大小端的问题
- mysql 增加超时的参数设置
- 修复 tc_bitmap 异常字符串处理的 bug
- 更新 protocol 引用

# v2.4.15 2020.12.30

### en

- fix tc_consistent_hash_new.cpp hash bug
- add ONLY_LIB, not build examples/servers by default, if you want build examples, please cmake .. -DONLY_LIB=ON && make -j4
- change format of some files to UTF-8 BOM to fix compile errors under windows without changing any codes in these files.
- fix tc_common prime judgment bug
- fix tars-tools.cmake link pthread bug in ubuntu

### cn

- 修改 tc_consistent_hash_new.cpp hash 的 bug
- 添加 ONLY_LIB 编译选项, 默认不再编译 examples 服务, 如果有需要请: cmake .. -DONLY_LIB=ON && make -j4
- 修改部分文件的 utf8 格式, 避免 windows 下的编译错误
- 修改 tc_common prime judgment bug
- 修改 ubuntu 下, tars-tools.cmake link pthread bug

## v2.4.14 2020.11.09

### en

- fix TC_Shm::init windows not set \_bOwner
- cmake copy .tars to tarsproto/protocol in windows
- epoll add/del/mod and return value
- ServantProxyThreadData reset when communicator terminate
- get rid of the singleton. You can start multiple applications in a process!
- when epoll server exit, close all fd.
- fix EndpointManager decontructor
- fix communicator terminate
- application support parse string config
- fix hash bug in first connection
- fix tc_sha, \_\_LITTLE_ENDIAN bug in windows/mac
- hello server add hash test
- fix compiler warning
- tc_shm when detach, set \_pshm = NULL

### cn

- 修改 TC_Shm::init in windows 没有设置 \_bOwner 变量的 bug
- 修改 tars-tools.cmake, 在 windows 平台下 cmake 复制 .tars 到 tarsproto/protocol
- epoll add/del/mod 等接口增加返回值
- communicator 结束时调用 ServantProxyThreadData reset
- 尽量去掉 libservant 中的单例, 这样在一个进程中可以启动多个 Application
- 当 epoll server 退出时, 关闭所有的句柄
- 修复 EndpointManager 析构函数
- 修改 communicator 结束时可能的 crash
- Application 内部增加直接初始化函数, 支持 string 类型的配置
- 修改第一次创建连接时 hash 不准的 bug
- 修复 windows/mac 平台上, tc_sha, \_\_LITTLE_ENDIAN 不支持的 bug
- Hello server 增加 hash 测试
- 修复编译警告
- tc_shm 当调用 detach 时, 设置\_pshm = NULL

## v2.4.13 2020.10.11

### en

- Fix Windows compilation errors (missing bison / flex generated files)
- Network epoll notification handle is changed to UDP handle instead of socket pipe mode to save the number of handles
- Fix tars-tools.cmake support mysql link

### cn

- 修复 windows 编译错误(漏提交 bison/flex 生成的文件)
- 网络 epoll 通知句柄改成 udp 句柄, 不采用 socket pipe 模式, 节省句柄数量
- 修复 tars-tools.cmake 支持 mysql 的 link

## v2.4.12 2020.09.23

### en

- tc_timer fix compiler warning
- fix dye bug
- fix http keep-alive bug

### cn

- tc_timer 修复编译警告
- 修复染色的 bug
- 修复 http keep-alive bug

## v2.4.11 2020.09.18

### en

- fix gtest compiler bug
- fix epoll memory leak in windows
- fix tc_encoder crash bug
- fix tc_clientsocket bug
- fix tc_timer memory leak
- fix tc_openssl mode not be used
- fix TC\_ Http2 failure problem (header cannot use temporary variables)
- always set thread data host info
- Adding the initialization of enumeration value in the resetDefautlt function
- Part of the file encoding format is adjusted to utf8 BOM to ensure that there is no problem in Windows compilation
- tc_common add various date processing functions
- tc_common add floating point comparison function
- tc_thread add multi thread hash logic
- tc_port add environment variable acquisition and system event registration callback (compatible with multi platform)
- Adjust tc_sem_mutex event name (under Windows) to facilitate subsequent maintenance
- rpc add support for http/sock4/sock5 proxy
- application add ssl bind check
- Fix the bug of RPC in HTTP short SSL package connection (under Windows)
- communicator add update endpoint
- fix communicator exit logic to avoid possible crash in exit
- optimize the memory copy problem, reduce the memory copy, and improve the performance
- Adjust the custom callback and put it in servantproxy

### cn

- 修复 gtest 编译错误
- 修复 windows 下, epoll.cpp 中的内存泄露
- 修改 tc_encoder crash 的 bug
- 修复 tc_clientsocket 在大包下的 bug
- 修复 tc_timer 内存泄露问题
- 修复 tc_openssl 代码中 mode 没生效的 bug
- 修复 rpc 在 http 短连接以及 ssl 大包下的 bug(windows 下)
- 修复 tc_http2 失败的问题(header 不能用临时变量)
- ServantProxy 调用时总是在线程私有变量中设置 host info
- 在 resetDefautlt 函数中增加枚举值的初始化
- 部分文件编码格式调整成 utf8 bom
- tc_common 增加各类时间处理函数
- tc_common 增加浮点数比较算法
- tc_thread 增加多线程 hash 逻辑
- tc_port 增加环境变量获取以及系统事件注册回调(兼容多平台)
- 调整 tc_sem_mutex 事件名称(windows 下), 便于后续维护
- rpc 增加 http/sock4/sock5 支持
- application 增加 ssl 效验
- communicator 增加主动更新 endpoint 的功能
- communicator 完善退出逻辑, 避免退出时可能的 crash
- 进一步优化内存 copy 问题, 减少内存 copy,提升性能
- 调整自定义回调, 放在 ServantProxy 中

## v2.4.10 2020.09.02

### en

- fix ssl EndpointManager.cpp bug
- gbk2utf8 mode default set to ICONV_IGNORE

### cn

- 修复 ssl 下, EndpointManager.cpp 的 bug
- 将 gbk2utf8 mode 缺省值设置为 ICONV_IGNORE

## v2.4.9 2020.08.19

### en

- Fix tars-tool.cmake `make upload` bug in linux
- Update tars-tools.cmake, support multi tars file
- tc_encoder optimize
- fix servant http call context release bug
- fix servant connection reconnect dead cycle
- fix http servant set timeout bug
- fix windows roll log bug
- fix tc_time postAtTime bug
- remove TARS_SSL in header
- fix windows gtest build bug

### cn

- 修改 tars-tool.cmake `make upload` 在 linux 上 bug, cmake 用${CMAKE_COMMAND}代替
- 修改 tars-tools.cmake, 支持多个 tars 文件
- 优化 tc_encoder
- 修复 http servant 短连接模式下 context 释放的 bug(貌似只会在 windows 下出现)
- 修复 prx 重连可能导致的死循环的问题
- 修复 http servant 设置 timeout 不正确的问题
- 修复 windows 下循环日志不启用的问题
- 修复 tc_time postAtTime 的 bug
- 头文件中删除 TARS_SSL 的宏, 避免内存模型不一致
- 修改 windows gtest 编译连接不争取的问题

## v2.4.8 2020.08.05

### en

- add tarsmerge, can merge multi tars file to one
- fix tars-tools.cmake, support: make xxxx-upload-tars, upload tars to web, for test
- fix when http header has not content-length, tc_http decode bug
- fix refresh endpoint crash

### cn

- 添加 tarsmerge 工具, 支持合并多个 tars 文件到一个
- 修改 tars-tools.cmake, 支持 make xxx-upload-tars, 上传本服务的 tars 文件到 web, 方便自动测试
- 修改当 http 头没有 content-length 时的解析 bug
- 修改 endpoint 更新时可能导致的 crash

## v2.4.7 2020.07.19

### en

- fix tars-tools.cmake, support ssl link order bug
- tars2cpp delete routerkey support (for grid, not use now)
- epoll server add onNewClient callback
- fix tc_gzip memory leak
- solve the default threads num problem

### cn

- tars-tools.cmake, 修改 ssl 连接顺序不对的问题
- tars2cpp 去掉 routerkey(给灰度使用, 目前版本都不再支持)
- epoll server 增加 onNewClient 回调
- fix tc_gzip 在某情况下的内存泄露
- 修改 threads 缺省参数值为 0 的 bug

## v2.4.6 2020.07.09

### en

- fix http parse repsonse bug to support TarsGateway

### cn

- 修改 http 解析响应包的 bug

## v2.4.5 2020.07.07

### en:

- tars2node support JSON_VERSION.

### cn:

- tars2node 工具支持 JSON_VERSION 协议。

## v2.4.4 2020.06.19

### en:

- add totalResponse for all protocol response;
- modity tars2cpp for json protocol: gen json protocol default && gen async_response_xx json protocol;

### cn:

- 增加 totalResponse，对 tars 所有协议（tars、tup、json）回包都支持；
- 修改 tars2cpp，默认都生成 json 协议支持，并修改了 async_response_xx 也支持 json 回包。

## v2.4.3 2020.06.10

### en:

- tc_spin_lock support arm
- fix tc_http security problem
- fix tc_mysql table name with database name bug
- tarscpp create tars protocol, default package optional field
- fix tars2case 的 bug

### cn:

- tc_spin_lock 支持 arm
- tc_http 解析安全问题
- tc_mysql 表名带 db 名称时 bug 的问题
- tarscpp 协议缺省值默认打包
- fix tars2case 的 bug

## v2.4.2 2020.06.02

### en:

- Add English commet to some header file commet
- tars2case support json format
- fix tc_http bug, ignore last header line
- fix tc_json null bug and add interface

### cn:

- 部分头文件注释增加英文注释
- tars2case 支持 json 格式
- 修复 tc_http 解析头, 漏掉最后一行的 bug
- 修复 tc_json null 的 bug 以及修改 tc_json 的函数接口, 更易用

## v2.4.1 2020.05.06

### en:

1 fix windows tars proto, define long bug, change to long long
2 fix windows, longlong tars_ntohll bug
3 fix windows, tc_ex system message pointer may be null cause crash
4 fix windows tc_epoll_server when send pending add epoll mod

### cn:

1 修复 windows 64 位 tars 协议中, long 定义的问题(为 32 位,改成 long long)
2 修复 windows 64 位整形, 字节序转换的问题
3 修复 windows tc_ex 中系统消息指针为 NULL 的 bug
4 修复 windows windows tc_epoll_server when send pending add epoll mod

## v2.4.0 2020.04.24

### en:

- fix tars2json bug of array
- TC_EpollServer::Connction add \_authInit initialize
- fix tarsnotify nodename bug
- compiler & install framework/\*.tars in tarscpp
- fix tars-tools.cmake 'make tar' bug in linux
- In epoll server, change tc_cas_queue to tc_thread_queue
- tc_thread queue add front & pop_front
- add tars to xml support , add tc_xml.h/cpp tool class
- fix tc_json bug in float/double
- tars2cpp add interface to get/set sql
- TarsOutpuStream member change to public
- add tars2case for benchmark

### cn:

- 修复 tars2json 中 array 的 bug
- TC_EpollServer::Connction 构造时，增加对\_authInit 变量初始化
- 修复 tarsnotify nodename bug
- compiler & install framework/\*.tars in tarscpp
- 修复 tars-tools.cmake make tar 在 linux 的 bug
- epoll server 中 tc_cas_queue 改为 tc_thread_queue, tc_cas_queue 在极端压力下可能会有问题, 原因待查
- tc_thread queue add front & pop_front
- 增加 tars to xml 的支持, 增加 tc_xml.h/cpp 解析类
- 修复 tc_json 的 float/double 的 bug
- tars2cpp 工具增加将结构体转换成 sql 的接口
- TarsOutpuStream 成员变量改成 public, 便于外部访问
- 增加 tars2case 工具, 给 benchmark 工具使用

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

- cookies 修改实现方式
- 为支持短连接(http), ServantProxy 增加连接串行模式
- rpc 支持 http1 客户端
- 统一了 http1 和 http2 的接口(http2 不在兼容之前的, 之前的设计上是有 bug 的)
  优化 http 性能
- 修改 Tars 开头的文件名以及部分类名, 方便代码合并
- 改进服务 ip list 切换的逻辑

## v2.2.0 2020.04.02

## en

- fix epoll bug in windows

## cn

- 修复 epoll 在 windows 下的 bug

## v2.1.2 2020.03.28

### en:

- Fix bugs in tar-tools.cmake's support for make release
- cmake_tars_server.sh Permissions are automatically set to executable during installation
- fix tc_common::strto bug in unsigned char
- Download and compile curl automatically under windows to support the subsequent make upload
- Repair the support of tars-tools.cmake for make upload under Windows

### cn:

- 修复 tars-tools.cmake 对 release 支持的 bug
- cmake_tars_server.sh 等安装时权限都自动设置为可执行
- tc_common::strto 修复 unsigned char 的 bug
- windows 下自动下载编译 curl,为后续 make upload 提供支持
- 修复 windows 下 tars-tools.cmake 对 make upload 的支持

## v2.1.1 2020.03.23

### en:

- update tars2node, support ts
- tc_json add get
- fix CMakeLists.txt in servant demo

### cn:

- 更新 tars2node, 完善对 ts 的支持
- tc_json 增加一个 api
- 修改了脚本创建服务后, CMakeLists.txt 模板, 默认管理 ssl http2

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

- 修改 udp 在 ipv6 下的 bug
- 服务框架增加 tars.resource 命令字, 可以查看服务内网络连接细节
- 修改响应队列上报的 bug
- 增加发送 buffer 过载保护
- 调整第三方库安装后的库路径
- tc_network_buffer 增加 iterator 接口
- 优化 tc_http tc_http tc_http_async 性能, 提升 http parser
  f- ix tars2node, support --with-tars

## v2.0.0 2020.03.03

- support linux mac windows
- support ARM
- Reduce memory copy and improve performance
- Download compile dependencies automatically
- fix epoll server dead cycle bug
- Improve compilation mode and rely on MySQL client Lib
- fix some bug
