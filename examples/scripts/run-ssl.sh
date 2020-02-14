#!/bin/bash

echo "run-ssl.sh"

killall -2 SSLServer

sleep 1
echo "start server: ./bin/SSLServer --config=../examples/SSLDemo/Server/config.conf &"

./bin/SSLServer --config=../examples/SSLDemo/Server/config.conf &

sleep 1

echo "client: ./bin/SSLClient"

./bin/SSLClient --config=../examples/SSLDemo/Client/config.conf --count=10000 --thread=2 --call=sync  --buffersize=1000 --netthread=1
./bin/SSLClient --config=../examples/SSLDemo/Client/config.conf --count=10000 --thread=2 --call=async  --buffersize=1000 --netthread=1

sleep 1

killall -2 SSLServer


