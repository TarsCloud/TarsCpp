#!/bin/bash

echo "run-co.sh"

EXE_PATH=$1
SRC_PATH=$2/..

echo ${EXE_PATH} ${SRC_PATH}

killall -2 CoroutineDemoAServer 
sleep 1

echo "start server: ${EXE_PATH}/CoroutineDemoAServer --config=${SRC_PATH}/examples/CoroutineDemo/AServer/config.conf &"

${EXE_PATH}/CoroutineDemoAServer --config=${SRC_PATH}/examples/CoroutineDemo/AServer/config.conf &

#-------------------------------------------------------------------------------------------------------

killall -2 CoroutineDemoBServer 
sleep 1

echo "start server: ${EXE_PATH}/CoroutineDemoBServer --config=${SRC_PATH}/examples/CoroutineDemo/BServer/config.conf &"

${EXE_PATH}/CoroutineDemoBServer --config=${SRC_PATH}/examples/CoroutineDemo/BServer/config.conf &


#-------------------------------------------------------------------------------------------------------
sleep 1

echo "client: ${EXE_PATH}/CoroutineDemoClient"

${EXE_PATH}/CoroutineDemoClient --count=10000 --call=serial --thread=2 --buffersize=100 --netthread=2

${EXE_PATH}/CoroutineDemoClient --count=10000 --call=parallel --thread=2 --buffersize=100 --netthread=2

${EXE_PATH}/testCoro 1000

${EXE_PATH}/testParallelCoro 1000

#-------------------------------------------------------------------------------------------------------

sleep 1

killall -9 CoroutineDemoAServer
killall -9 CoroutineDemoBServer


