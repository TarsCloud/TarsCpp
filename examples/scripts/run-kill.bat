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


