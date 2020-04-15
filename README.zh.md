Tars整体介绍文档请阅读: https://tarscloud.gitbook.io/

该工程是Tars RPC框架C++语言的源代码

目录名称 |功能
------------------|----------------
[servant](https://github.com/TarsCloud/TarsCpp/tree/master/servant)      |C++语言框架rpc的源码实现
[tools](https://github.com/TarsCloud/TarsCpp/tree/master/tools)        |C++语言框架IDL工具的源码实现
[util](https://github.com/TarsCloud/TarsCpp/tree/master/util)          |C++语言框架基础工具库的源码实现
[examples](https://github.com/TarsCloud/TarsCpp/tree/master/examples)     |C++语言框架的示例代码，包括：快速入门示例、promise编程入门示例、压测程序示例
[unittest](https://github.com/TarsCloud/tars-unittest/tree/master)      |tars cpp rpc框架的单元测试用例，基于GoogleTest开发；可以使用命令'git submodule init unittest;git submodule update'作为git子模块下载
[test_deprecated](https://github.com/TarsCloud/TarsCpp/tree/master/test)         |C++语言框架各个部分的测试程序,已弃用
[docs](https://github.com/TarsCloud/TarsCpp/tree/master/docs)         |文档说明
[docs-en](https://github.com/TarsCloud/TarsCpp/tree/master/docs-en)      |英文文档说明

依赖环境

软件 |软件要求
------|--------
linux内核版本:      |	2.6.18及以上版本
gcc版本:          	|   4.1.2及以上版本、glibc-devel
bison工具版本:      |	2.5及以上版本
flex工具版本:       |	2.5及以上版本
cmake版本：       	|   3.2及以上版本
mysql版本：         |	4.1.17及以上版本

编译和安装
```
git clone https://github.com/TarsCloud/TarsCpp.git --recursive
cd TarsCpp
cmake .
make
make install
```

详细安装参见[案例文档](https://tarscloud.github.io/TarsDocs/)
