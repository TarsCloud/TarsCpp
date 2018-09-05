[点我查看中文版](README.zh.md)

This project is the source code of the Tars RPC framework C++ language.

Directory |Features
------------------|----------------
servant      |Source code implementation of C++ language framework rpc
tools        |Source code implementation of C++ language framework IDL tool
util         |Source code implementation of C++ language framework basic tool library
examples     |Sample code for the C++ language framework, including: quick start examples, introduction to promise programming, examples of pressure test programs
test         |Test procedures for various parts of the C++ language framework
docs         |Document description
docs-en      |English document description

Dependent environment

Software |version requirements
------|--------
linux kernel:   |	2.6.18 and above
gcc:          	|   4.1.2 and above glibc-devel
bison tool:     |	2.5 and above
flex tool:      |	2.5 and above
cmake:       	|   2.8.8 and above
mysql:          |	4.1.17 and above

Compile and install
```
cmake .
make
make install
```

Detailed installation reference Install.md
