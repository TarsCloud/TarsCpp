#!/bin/bash

echo "run-http2.sh"

killall -2 Http2Server

sleep 1
echo "start server: ./bin/Http2Server --config=../examples/HttpDemo/Http2Server/config.conf &"

./bin/Http2Server --config=../examples/HttpDemo/Http2Server/config.conf &

sleep 1

echo "client: ./bin/Http2Client"

./bin/Http2Client --count=10000 --thread=2 --call=async
./bin/Http2Client --count=10000 --thread=2 --call=sync

sleep 1

killall -2 Http2Server


