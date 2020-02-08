#!/bin/bash

echo "run-custom.sh"

killall -2 CustomServer 

sleep 1
echo "start server: ./bin/CustomServer --config=../examples/CustomDemo/CustomServer/config.conf &"

./bin/CustomServer --config=../examples/CustomDemo/CustomServer/config.conf &

sleep 2

#-------------------------------------------------------------------------------------------------------

echo "client: ./bin/CustomClient"

./bin/CustomClient 5

killall -2 CustomServer



