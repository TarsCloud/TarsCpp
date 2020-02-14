#!/bin/bash

echo "run-kill.sh"

killall -9 SSLServer
killall -9 AuthServer
killall -9 CoroutineDemoAServer
killall -9 CoroutineDemoBServer
killall -9 CustomServer
killall -9 HttpServer
killall -9 Http2Server
killall -9 PushServer
killall -9 QuickStartDemo
killall -9 ProxyServer

