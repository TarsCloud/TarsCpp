#!/bin/bash

echo "run-http.sh"

killall -2 HttpServer 

sleep 1
echo "start server: ./bin/HttpServer --config=../examples/HttpDemo/HttpServer/config.conf &"

./bin/HttpServer --config=../examples/HttpDemo/HttpServer/config.conf &

sleep 1

echo "client: ./bin/HttpClient"

./bin/HttpClient 2 10000

sleep 1

killall -2 HttpServer 


