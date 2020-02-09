#!/bin/bash

echo "run-http.sh"

killall -2 HttpServer 

sleep 1
echo "start server: ./bin/HttpServer --config=../examples/HttpDemo/HttpServer/config.conf &"

./bin/HttpServer --config=../examples/HttpDemo/HttpServer/config.conf &

sleep 1

echo "client: ./bin/HttpClient"

./bin/HttpClient --count=10000 --thread=2 --call=basehttp
./bin/HttpClient --count=10000 --thread=2 --call=synchttp
./bin/HttpClient --count=10000 --thread=2 --call=asynchttp

sleep 1

killall -2 HttpServer 


