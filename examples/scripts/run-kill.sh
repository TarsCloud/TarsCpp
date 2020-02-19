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

killall -9 HttpClient
killall -9 Http2Client
killall -9 AuthClient
killall -9 SSLClient
killall -9 CoroutineDemoClient
killall -9 testCoro
killall -9 testParallelCoro
killall -9 CustomClient
killall -9 PushClient
killall -9 QuickStartDemoClient