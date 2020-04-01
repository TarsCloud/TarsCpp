#!/bin/bash

echo "run-co.sh"
EXE_PATH=$1
SRC_PATH=$2/..

echo ${EXE_PATH} ${SRC_PATH}

killall -9 PushServer

sleep 1
echo "start server: ${EXE_PATH}/PushServer --config=${SRC_PATH}/examples/PushDemo/PushServer/config.conf &"

${EXE_PATH}/PushServer --config=${SRC_PATH}/examples/PushDemo/PushServer/config.conf &

sleep 2

#-------------------------------------------------------------------------------------------------------

echo "client: ${EXE_PATH}/PushClient"

${EXE_PATH}/PushClient 5

killall -9 PushServer



