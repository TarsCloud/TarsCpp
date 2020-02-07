#!/bin/bash

echo "run-co.sh"


killall -2 CoroutineDemoAServer 
sleep 1

echo "start server: ./bin/CoroutineDemoAServer --config=../examples/CoroutineDemo/AServer/config.conf &"

./bin/CoroutineDemoAServer --config=../examples/CoroutineDemo/AServer/config.conf &

#-------------------------------------------------------------------------------------------------------

killall -2 CoroutineDemoBServer 
sleep 1

echo "start server: ./bin/CoroutineDemoBServer --config=../examples/CoroutineDemo/BServer/config.conf &"

./bin/CoroutineDemoBServer --config=../examples/CoroutineDemo/BServer/config.conf &


#-------------------------------------------------------------------------------------------------------
sleep 1

echo "client: ./bin/CoroutineDemoClient"

./bin/CoroutineDemoClient 2 10000 0

./bin/CoroutineDemoClient 2 10000 1

./bin/testCoro 1000

./bin/testParallelCoro 1000

#-------------------------------------------------------------------------------------------------------

sleep 1

killall -2 CoroutineDemoAServer 
killall -2 CoroutineDemoBServer 


