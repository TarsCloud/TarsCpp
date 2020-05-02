#!/bin/bash

echo "run-http.sh"

EXE_PATH=$1
SRC_PATH=$2/..

echo ${EXE_PATH} ${SRC_PATH}

killall -9 HttpServer

sleep 1
echo "start server: ${EXE_PATH}/HttpServer --config=${SRC_PATH}/examples/HttpDemo/HttpServer/config.conf &"

${EXE_PATH}/HttpServer --config=${SRC_PATH}/examples/HttpDemo/HttpServer/config.conf &

sleep 1

echo "client: ${EXE_PATH}/HttpClient"

${EXE_PATH}/HttpClient --count=10000 --thread=2 --call=base
${EXE_PATH}/HttpClient --count=10000 --thread=2 --call=sync
${EXE_PATH}/HttpClient --count=10000 --thread=2 --call=async
#${EXE_PATH}/HttpClient --count=10000 --thread=2 --call=synchttp

sleep 1

killall -9 HttpServer


