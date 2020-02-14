#!/bin/bash

echo "run-auth.sh"

killall -2 AuthServer

sleep 1
echo "start server: ./bin/AuthServer --config=../examples/AuthDemo/Server/config.conf &"

./bin/AuthServer --config=../examples/AuthDemo/Server/config.conf &

sleep 1

echo "client: ./bin/AuthClient"

./bin/AuthClient --config=../examples/AuthDemo/Client/config.conf --count=10000 --thread=2 --call=sync  --buffersize=1000 --netthread=1
./bin/AuthClient --config=../examples/AuthDemo/Client/config.conf --count=10000 --thread=2 --call=async  --buffersize=1000 --netthread=1

sleep 1

killall -2 AuthServer


