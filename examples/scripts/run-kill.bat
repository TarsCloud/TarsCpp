#!/bin/bash

echo "run-kill.bat"

taskkill /im SSLServer.exe /f
taskkill /im AuthServer.exe /f
taskkill /im CoroutineDemoAServer.exe /f
taskkill /im CoroutineDemoBServer.exe /f
taskkill /im CustomServer.exe /f
taskkill /im HttpServer.exe /f
taskkill /im Http2Server.exe /f
taskkill /im PushServer.exe /f
taskkill /im QuickStartDemo.exe /f
taskkill /im ProxyServer.exe /f
taskkill /im busybox.exe /f

taskkill /im HttpClient.exe /f
taskkill /im Http2Client.exe /f
taskkill /im AuthClient.exe /f
taskkill /im SSLClient.exe /f
taskkill /im CoroutineDemoClient.exe /f
taskkill /im testCoro.exe /f
taskkill /im testParallelCoro.exe /f
taskkill /im CustomClient.exe /f
taskkill /im PushClient.exe /f
taskkill /im QuickStartDemoClient.exe /f