[点我查看中文版](README.zh.md)

This project is the source code of the rpc implementation of the Tars framework C++ language.

Directory |Features
------------------|----------------
wup                   |Uniform communication protocol (tars/wup protocol) of rpc framework and codec implementation of protocol
protocol              |The return code of the underlying communication defined by the rpc framework and the communication interface file that interacts with the framework underlying service
servant/libservant    |Source code implementation of rpc framework
jmem                  |Source code implementation of memory data structure component based on tars protocol
promise               |Source implementation based on promise asynchronous programming
makefile              |The makefile implementation of the compiled source code using the TARS framework C++ language
script                |Script tool to generate TARS service template code
