#!/bin/bash

echo "run-push-callback.sh"

EXE_PATH=$1
SRC_PATH=$2/..

echo ${EXE_PATH} ${SRC_PATH}

killall -9 PushCallbackServer

sleep 1
echo "start server: ${EXE_PATH}/PushCallbackServer --config=${SRC_PATH}/examples/PushCallbackDemo/Server/config.conf &"

${EXE_PATH}/PushCallbackServer --config=${SRC_PATH}/examples/PushCallbackDemo/Server/config.conf &

sleep 1

echo "client: ${EXE_PATH}/PushCallbackClient"

${EXE_PATH}/PushCallbackClient --config=${SRC_PATH}/examples/PushCallbackDemo/Client/config.conf

sleep 1

killall -9 PushCallbackServer


