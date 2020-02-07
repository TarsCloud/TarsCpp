#!/bin/bash

echo "run-quick-start.sh"

killall -2 QuickStartDemo ProxyServer

sleep 1
echo "start server: ./bin/QuickStartDemo --config=../examples/QuickStartDemo/HelloServer/Server/config.conf &"

./bin/QuickStartDemo --config=../examples/QuickStartDemo/HelloServer/Server/config.conf &
./bin/ProxyServer --config=../examples/QuickStartDemo/ProxyServer/Server/config.conf &

sleep 2

echo "client: ./bin/QuickStartDemoClient"

./bin/QuickStartDemoClient --count=100000 --call=sync --thread=2 --buffersize=100 --netthread=2

./bin/QuickStartDemoClient --count=100000 --call=async --thread=2 --buffersize=100 --netthread=2

./bin/QuickStartDemoClient --count=100000 --call=synctup --thread=2 --buffersize=100 --netthread=2

./bin/QuickStartDemoClient --count=100000 --call=asynctup --thread=2 --buffersize=100 --netthread=2

echo "client: ./bin/ProxyServerClient"

./bin/ProxyServerClient

sleep 1

killall -2 ProxyServer QuickStartDemo


