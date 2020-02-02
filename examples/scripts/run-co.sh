#!/bin/bash

echo "run-co.sh"

WORKDIR=$(pwd)

echo ${WORKDIR}

killall -9 CoroutineDemoAServer 

echo "start server: ${WORKDIR}/../bin/CoroutineDemoAServer --config=${WORKDIR}/../../examples/CoroutineDemo/AServer/config.conf &"

${WORKDIR}/../bin/CoroutineDemoAServer --config=${WORKDIR}/../../examples/CoroutineDemo/AServer/config.conf &

#-------------------------------------------------------------------------------------------------------

killall -9 CoroutineDemoBServer 

echo "start server: ${WORKDIR}/../bin/CoroutineDemoBServer --config=${WORKDIR}/../../examples/CoroutineDemo/BServer/config.conf &"

${WORKDIR}/../bin/CoroutineDemoBServer --config=${WORKDIR}/../../examples/CoroutineDemo/BServer/config.conf &


#-------------------------------------------------------------------------------------------------------
sleep 3


echo "client: ${WORKDIR}/../bin/CoroutineDemoClient"

${WORKDIR}/../bin/CoroutineDemoClient 2 10000 0

${WORKDIR}/../bin/CoroutineDemoClient 2 10000 1

${WORKDIR}/../bin/testCoro 1000

${WORKDIR}/../bin/testParallelCoro 1000

#-------------------------------------------------------------------------------------------------------

#killall -9 CoroutineDemoAServer 
#killall -9 CoroutineDemoBServer 


