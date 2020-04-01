#!/bin/bash

echo "run-quick-start.sh"
EXE_PATH=$1
SRC_PATH=$2/..

echo ${EXE_PATH} ${SRC_PATH}

killall -9 QuickStartDemo ProxyServer

sleep 1
echo "start server: ${EXE_PATH}/QuickStartDemo --config=${SRC_PATH}/examples/QuickStartDemo/HelloServer/Server/config.conf &"

${EXE_PATH}/QuickStartDemo --config=${SRC_PATH}/examples/QuickStartDemo/HelloServer/Server/config.conf &
${EXE_PATH}/ProxyServer --config=${SRC_PATH}/examples/QuickStartDemo/ProxyServer/Server/config.conf &

sleep 2

echo "client: ${EXE_PATH}/QuickStartDemoClient"

${EXE_PATH}/QuickStartDemoClient --count=100000 --call=sync --thread=2 --buffersize=100 --netthread=2

${EXE_PATH}/QuickStartDemoClient --count=100000 --call=async --thread=2 --buffersize=100 --netthread=2

${EXE_PATH}/QuickStartDemoClient --count=100000 --call=synctup --thread=2 --buffersize=100 --netthread=2

${EXE_PATH}/QuickStartDemoClient --count=100000 --call=asynctup --thread=2 --buffersize=100 --netthread=2

echo "client: ${EXE_PATH}/ProxyServerClient"

${EXE_PATH}/ProxyServerClient

sleep 1

killall -9 ProxyServer QuickStartDemo


