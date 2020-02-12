#!/bin/sh

echo "run-push.bat"

killall -9 CustomServer.exe 
sleep 1

echo "start server: .\\bin\\Release\\CustomServer.exe --config=..\\examples\\CustomDemo\\CustomServer\\config.conf &"

.\\bin\\Release\\CustomServer.exe --config=..\\examples\\CustomDemo\\CustomServer\\config.conf &

sleep 3

#-------------------------------------------------------------------------------------------------------

echo "client: .\\bin\\Release\\CustomClient.exe"

.\\bin\\Release\\CustomClient.exe --count=10000 --thread=2 --call=sync --netthread=2 --buffersize=100
.\\bin\\Release\\CustomClient.exe --count=10000 --thread=2 --call=async --netthread=2 --buffersize=100

killall -9 CustomServer.exe

