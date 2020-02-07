#!/bin/bash

echo "run-co.sh"

killall -2 PushServer 

sleep 1
echo "start server: ./bin/PushServer --config=../examples/PushDemo/PushServer/config.conf &"

./bin/PushServer --config=../examples/PushDemo/PushServer/config.conf &

sleep 2

#-------------------------------------------------------------------------------------------------------

echo "client: ./bin/PushClient"

./bin/PushClient 5

killall -2 PushServer



