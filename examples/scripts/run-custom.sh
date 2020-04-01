#!/bin/bash

echo "run-custom.sh"

EXE_PATH=$1
SRC_PATH=$2/..

echo ${EXE_PATH} ${SRC_PATH}

killall -9 CustomServer

sleep 1
echo "start server: ${EXE_PATH}/CustomServer --config=${SRC_PATH}/examples/CustomDemo/CustomServer/config.conf &"

${EXE_PATH}/CustomServer --config=${SRC_PATH}/examples/CustomDemo/CustomServer/config.conf &

sleep 2

#-------------------------------------------------------------------------------------------------------

echo "client: ${EXE_PATH}/CustomClient"

${EXE_PATH}/CustomClient --count=10000 --thread=2 --call=sync --netthread=1 --buffersize=100

${EXE_PATH}/CustomClient --count=10000 --thread=2 --call=async --netthread=1 --buffersize=100

killall -9 CustomServer



