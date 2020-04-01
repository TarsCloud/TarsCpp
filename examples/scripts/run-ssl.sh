#!/bin/bash

echo "run-ssl.sh"
EXE_PATH=$1
SRC_PATH=$2/..

echo ${EXE_PATH} ${SRC_PATH}

killall -9 SSLServer

sleep 1
echo "start server: ${EXE_PATH}/SSLServer --config=${SRC_PATH}/examples/SSLDemo/Server/config.conf &"

${EXE_PATH}/SSLServer --config=${SRC_PATH}/examples/SSLDemo/Server/config.conf &

sleep 1

echo "client: ${EXE_PATH}/SSLClient"

${EXE_PATH}/SSLClient --config=${SRC_PATH}/examples/SSLDemo/Client/config.conf --count=10000 --thread=2 --call=sync  --buffersize=100 --netthread=1
${EXE_PATH}/SSLClient --config=${SRC_PATH}/examples/SSLDemo/Client/config.conf --count=10000 --thread=2 --call=async  --buffersize=100 --netthread=1

${EXE_PATH}/SSLClient --config=${SRC_PATH}/examples/SSLDemo/Client/config.conf --count=10000 --thread=2 --call=sync1 --buffersize=100 --netthread=1
${EXE_PATH}/SSLClient --config=${SRC_PATH}/examples/SSLDemo/Client/config.conf --count=10000 --thread=2 --call=async1  --buffersize=100 --netthread=1

${EXE_PATH}/SSLClient --config=${SRC_PATH}/examples/SSLDemo/Client/config.conf --count=10000 --thread=2 --call=sync2 --buffersize=100 --netthread=1
${EXE_PATH}/SSLClient --config=${SRC_PATH}/examples/SSLDemo/Client/config.conf --count=10000 --thread=2 --call=async2  --buffersize=100 --netthread=1

${EXE_PATH}/SSLClient --config=${SRC_PATH}/examples/SSLDemo/Client/config.conf --count=10000 --thread=2 --call=sync3 --buffersize=100 --netthread=1
${EXE_PATH}/SSLClient --config=${SRC_PATH}/examples/SSLDemo/Client/config.conf --count=10000 --thread=2 --call=async3  --buffersize=100 --netthread=1

sleep 1

killall -9 SSLServer


