cmake_minimum_required(VERSION 3.16)

project(DemoApp-DemoServer)

gen_server(DemoApp DemoServer)

#target_link_libraries(mysqlclient ssl crypto nghttp2-static)
