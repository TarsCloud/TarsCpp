#!/bin/bash

echo "run-auth.sh"

EXE_PATH=$1
SRC_PATH=$2/..

echo ${EXE_PATH} ${SRC_PATH}

killall -9 AuthServer

sleep 1
echo "start server: ${EXE_PATH}/AuthServer --config=${SRC_PATH}/examples/AuthDemo/Server/config.conf &"

${EXE_PATH}/AuthServer --config=${SRC_PATH}/examples/AuthDemo/Server/config.conf &

sleep 1

echo "client: ${EXE_PATH}/AuthClient"

${EXE_PATH}/AuthClient --config=${SRC_PATH}/examples/AuthDemo/Client/config.conf --count=10000 --thread=2 --call=sync  --buffersize=1000 --netthread=1
${EXE_PATH}/AuthClient --config=${SRC_PATH}/examples/AuthDemo/Client/config.conf --count=10000 --thread=2 --call=async  --buffersize=1000 --netthread=1

sleep 1

killall -9 AuthServer


