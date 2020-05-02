#!/bin/bash

echo "run-http2.sh"
EXE_PATH=$1
SRC_PATH=$2/..

echo ${EXE_PATH} ${SRC_PATH}

killall -9 Http2Server

sleep 1
echo "start server: ${EXE_PATH}/Http2Server --config=${SRC_PATH}/examples/HttpDemo/Http2Server/config.conf &"

${EXE_PATH}/Http2Server --config=${SRC_PATH}/examples/HttpDemo/Http2Server/config.conf &

sleep 1

echo "client: ${EXE_PATH}/Http2Client"

${EXE_PATH}/Http2Client --count=10000 --thread=2 --call=async
${EXE_PATH}/Http2Client --count=10000 --thread=2 --call=sync

sleep 1

killall -9 Http2Server


