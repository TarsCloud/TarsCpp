#!/bin/sh

echo "run-push.bat"

killall -9 CustomServer.exe 
sleep 1

echo "start server: .\\bin\\Release\\CustomServer.exe --config=..\\examples\\CustomDemo\\CustomServer\\config.conf &"

.\\bin\\Release\\CustomServer.exe --config=..\\examples\\CustomDemo\\CustomServer\\config.conf &

sleep 3

#-------------------------------------------------------------------------------------------------------

echo "client: .\\bin\\Release\\CustomClient.exe"

.\\bin\\Release\\CustomClient.exe 5

killall -9 CustomServer.exe

