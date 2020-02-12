#!/bin/bash

echo "run-custom.sh"

killall -2 CustomServer 

sleep 1
echo "start server: ./bin/CustomServer --config=../examples/CustomDemo/CustomServer/config.conf &"

./bin/CustomServer --config=../examples/CustomDemo/CustomServer/config.conf &

sleep 2

#-------------------------------------------------------------------------------------------------------

echo "client: ./bin/CustomClient"

./bin/CustomClient --count=10000 --thread=2 --call=sync --netthread=1 --buffersize=100

./bin/CustomClient --count=10000 --thread=2 --call=async --netthread=1 --buffersize=100

killall -2 CustomServer



