#!/bin/sh

echo "run-push.bat"

killall -2 PushServer.exe 
sleep 1

echo "start server: .\\bin\\Release\\PushServer.exe --config=..\\examples\\PushDemo\\PushServer\\config.conf &"

.\\bin\\Release\\PushServer.exe --config=..\\examples\\PushDemo\\PushServer\\config.conf &

sleep 3

#-------------------------------------------------------------------------------------------------------

echo "client: .\\bin\\Release\\PushClient.exe"

.\\bin\\Release\\PushClient.exe 5

killall -2 PushServe.exe

