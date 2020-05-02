#!/bin/bash

echo "run-auth.sh"

EXE_PATH=$1
SRC_PATH=$2/..

echo ${EXE_PATH} ${SRC_PATH}

killall -9 UdpServer

sleep 1
echo "start server: ${EXE_PATH}/UdpServer --config=${SRC_PATH}/examples/UdpDemo/Server/config.conf &"

${EXE_PATH}/UdpServer --config=${SRC_PATH}/examples/UdpDemo/Server/config.conf &

sleep 1

echo "client: ${EXE_PATH}/UdpClient"

${EXE_PATH}/UdpClient --config=${SRC_PATH}/examples/UdpDemo/Client/config.conf --count=10000 --thread=2 --call=sync  --buffersize=1000 --netthread=1
${EXE_PATH}/UdpClient --config=${SRC_PATH}/examples/UdpDemo/Client/config.conf --count=10000 --thread=2 --call=async  --buffersize=1000 --netthread=1

sleep 1

killall -9 UdpServer


