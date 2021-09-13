
# v2.4.21 2021.08.19
### en
- fix StatReport crash when communicatorEpoll delete
- update protocol to support K8S
- tars-tools.cmake support K8S upload
- tc_socket bind output to cerr when failed
- fix tc_json double bug
### cn
- 修复StatReport可能的crash当communicatorEpoll被delete时 
- 为了后续支持K8S更新了相关的协议
- 更新了tars-tools.cmake support K8S upload 
- tc_socket 增加了cerr的输出, 帮忙定位问题
- 修复tc_json double精度的bug
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
- Application 添加 addServantWithParams, 支持Servant构造参数
- 去掉Message.h opentracking 定义
- 修复tc_timer内存泄露和timer id可能冲突的问题
- 支持grpc GrpcDemo, 修复tc_grpc compiler
- 修复 windows 下 socket 设置阻塞异常
- 修复 refresh-endpoint-interval 配置无效
- 更新unittest

# v2.4.19 2021.05.18
### en
- fix TARS_OPENTARCKING not report trace bug
- update mysql-client to support cmake 3.20
- fix doc README.md 404
### cn
- 修复打开TARS_OPENTARCKING宏时，没有上报trace信息的问题
- update mysql-client to support cmake 3.20
- fix doc README.md 404

## v2.4.18 2021.04.13
### en
- fix tc_thread_queue,Optimizing the swarm effect
- fix epoll server connection close cause crash bug
### cn
- 修复tc_thread_queue 优化惊群效应
- 修复epoll server 连接关闭时可能带来的crash
# v2.4.17 2021.04.02
### en
- fix tc_time bug of erase caused by timer repeat event
- Fix the MySQL client library to solve the problem of compiling under cmake3.20
- Fix tc_config, key has no escaped bug 
- Fix the scheduling mechanism to avoid the problem of 1 s scheduling delay
### cn
- 修复tc_timer repeat事件带来的无法erase的问题
- 修复引用的mysql-client库, 解决cmake3.20下编译的问题
- 修复tc_config key没有转义的bug
- 修复协程调度机制, 避免出现总是出现1s调度延时的问题

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
- fix In Linux and windows, it is a small end. It corrects the definition error of byte order conversion function, such as tars_ ntonll
- set timeout options to prevent "connnect", "real_query" to be blocked for tens of minutes
- Fix the function name (tc_bitmap) in the exception
- update protocol
### cn
- tc_epoll_server 添加 sendBufferDirect 
- tc_epoll_server fd添加fork后自动close属性
- 修复 tc_common 中时区函数的时区的bug
- 修复hash第一次建立连接不稳定的bug
- 修复EndpoingManager.cpp 缺少 missing break的bug
- 修复服务退出时可能的crash
- 修复 popen 失败返回空指针 没有判断就直接使用的BUG。
- 修复解决服务节点权重更新时哈希表和一致性哈希环不更新的问题
- 修复同一服务有多个obj的情况，同一hash值调用不同的obj会hash到不同的服务器，因为addNode会根据desc(ip+port)计算md5,导致顺序不一致
- 修复处理压测连接数大于服务最大连接数,并且有连接频繁建立、断开时
- 修复windows下大小端的问题
- mysql 增加超时的参数设置
- 修复tc_bitmap异常字符串处理的bug
- 更新protocol引用

# v2.4.15 2020.12.30
### en
- fix tc_consistent_hash_new.cpp hash bug
- add ONLY_LIB, not build examples/servers by default, if you want build examples, please cmake .. -DONLY_LIB=ON && make -j4
- change format of some files to UTF-8 BOM to fix compile errors under windows without changing any codes in these files.
- fix tc_common prime judgment bug
- fix tars-tools.cmake link pthread bug in ubuntu
### cn
- 修改tc_consistent_hash_new.cpp hash的bug
- 添加ONLY_LIB编译选项, 默认不再编译examples服务, 如果有需要请: cmake .. -DONLY_LIB=ON && make -j4
- 修改部分文件的utf8格式, 避免windows下的编译错误
- 修改tc_common prime judgment bug
- 修改ubuntu下, tars-tools.cmake link pthread bug

## v2.4.14 2020.11.09
### en
- fix TC_Shm::init windows not set _bOwner
- cmake copy .tars to tarsproto/protocol in windows
- epoll add/del/mod and return value
- ServantProxyThreadData reset when communicator terminate
- get rid of the singleton. You can start multiple applications in a process!
- when epoll server exit, close all fd.
- fix EndpointManager decontructor
- fix communicator terminate
- application support parse string config
- fix hash bug in first connection
- fix tc_sha, __LITTLE_ENDIAN bug in windows/mac
- hello server add hash test
- fix compiler warning
- tc_shm when detach, set _pshm = NULL

### cn
- 修改 TC_Shm::init in windows 没有设置 _bOwner变量的bug
- 修改tars-tools.cmake, 在windows 平台下cmake 复制 .tars 到 tarsproto/protocol
- epoll add/del/mod 等接口增加返回值
- communicator 结束时调用ServantProxyThreadData reset
- 尽量去掉libservant中的单例, 这样在一个进程中可以启动多个Application
- 当epoll server退出时, 关闭所有的句柄
- 修复EndpointManager析构函数
- 修改communicator结束时可能的crash
- Application内部增加直接初始化函数, 支持string类型的配置
- 修改第一次创建连接时hash不准的bug
- 修复windows/mac平台上, tc_sha, __LITTLE_ENDIAN不支持的bug
- Hello server增加hash测试
- 修复编译警告
- tc_shm当调用detach时, 设置_pshm = NULL

## v2.4.13 2020.10.11
### en
- Fix Windows compilation errors (missing bison / flex generated files)
- Network epoll notification handle is changed to UDP handle instead of socket pipe mode to save the number of handles
- Fix tars-tools.cmake support mysql link
### cn
- 修复windows编译错误(漏提交bison/flex生成的文件) 
- 网络epoll通知句柄改成udp句柄, 不采用socket pipe模式, 节省句柄数量
- 修复tars-tools.cmake 支持mysql的link

## v2.4.12 2020.09.23
### en
- tc_timer fix compiler warning
- fix dye bug
- fix http keep-alive bug
### cn
- tc_timer修复编译警告
- 修复染色的bug
- 修复http keep-alive bug

## v2.4.11 2020.09.18
### en
- fix gtest compiler bug
- fix epoll memory leak in windows
- fix tc_encoder crash bug
- fix tc_clientsocket bug
- fix tc_timer memory leak
- fix tc_openssl mode not be used
- fix TC_ Http2 failure problem (header cannot use temporary variables)
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
- Fix the bug of RPC in HTTP short SSL package connection  (under Windows)
- communicator add update endpoint
- fix communicator exit logic to avoid possible crash in exit
- optimize the memory copy problem, reduce the memory copy, and improve the performance
- Adjust the custom callback and put it in servantproxy

### cn
- 修复gtest编译错误
- 修复windows下, epoll.cpp中的内存泄露
- 修改tc_encoder crash 的bug
- 修复tc_clientsocket在大包下的bug
- 修复tc_timer内存泄露问题
- 修复tc_openssl 代码中mode没生效的bug
- 修复rpc在http短连接以及ssl大包下的bug(windows下)
- 修复tc_http2 失败的问题(header不能用临时变量)
- ServantProxy调用时总是在线程私有变量中设置host info
- 在resetDefautlt函数中增加枚举值的初始化
- 部分文件编码格式调整成utf8 bom
- tc_common增加各类时间处理函数
- tc_common增加浮点数比较算法
- tc_thread增加多线程hash逻辑
- tc_port增加环境变量获取以及系统事件注册回调(兼容多平台)
- 调整tc_sem_mutex事件名称(windows下), 便于后续维护
- rpc增加http/sock4/sock5支持
- application 增加ssl效验
- communicator增加主动更新endpoint的功能
- communicator完善退出逻辑, 避免退出时可能的crash
- 进一步优化内存copy问题, 减少内存copy,提升性能
- 调整自定义回调, 放在ServantProxy中

## v2.4.10 2020.09.02
### en
- fix ssl EndpointManager.cpp bug 
- gbk2utf8 mode default set to ICONV_IGNORE
### cn
- 修复ssl下, EndpointManager.cpp 的bug 
- 将gbk2utf8 mode 缺省值设置为ICONV_IGNORE

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
- 修改tars-tool.cmake `make upload` 在linux上bug, cmake用${CMAKE_COMMAND}代替 
- 修改tars-tools.cmake, 支持多个tars文件
- 优化tc_encoder
- 修复http servant 短连接模式下context释放的bug(貌似只会在windows下出现)
- 修复prx重连可能导致的死循环的问题
- 修复http servant 设置timeout不正确的问题
- 修复windows下循环日志不启用的问题
- 修复tc_time postAtTime的bug
- 头文件中删除TARS_SSL的宏, 避免内存模型不一致
- 修改windows gtest编译连接不争取的问题

## v2.4.8 2020.08.05
### en
- add tarsmerge, can merge multi tars file to one
- fix tars-tools.cmake, support: make xxxx-upload-tars, upload tars to web, for test
- fix when http header has not content-length, tc_http decode bug
- fix refresh endpoint crash
### cn
- 添加 tarsmerge工具, 支持合并多个tars文件到一个
- 修改tars-tools.cmake, 支持make xxx-upload-tars, 上传本服务的tars文件到web, 方便自动测试
- 修改当http头没有content-length时的解析bug
- 修改endpoint更新时可能导致的crash

## v2.4.7 2020.07.19
### en
- fix tars-tools.cmake, support ssl link order bug
- tars2cpp delete routerkey support (for grid, not use now)
- epoll server add onNewClient callback
- fix tc_gzip memory leak
- solve the default threads num problem 
### cn
- tars-tools.cmake, 修改ssl 连接顺序不对的问题
- tars2cpp去掉routerkey(给灰度使用, 目前版本都不再支持)
- epoll server 增加onNewClient回调
- fix tc_gzip在某情况下的内存泄露
- 修改threads缺省参数值为0的bug

## v2.4.6 2020.07.09
### en
- fix http parse repsonse bug to support TarsGateway
### cn
- 修改http解析响应包的bug

## v2.4.5 2020.07.07
### en:
- tars2node support JSON_VERSION.
### cn:
- tars2node 工具支持JSON_VERSION 协议。

## v2.4.4 2020.06.19
### en:
- add totalResponse for all protocol response;
- modity tars2cpp for json protocol: gen json protocol default && gen async_response_xx json protocol;
### cn:
- 增加 totalResponse，对tars所有协议（tars、tup、json）回包都支持；
- 修改tars2cpp，默认都生成json协议支持，并修改了 async_response_xx 也支持json回包。

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