#!/bin/bash

echo "run-http.sh"

WORKDIR=$(pwd)

echo ${WORKDIR}

killall -9 HttpServer 

echo "start server: ${WORKDIR}/../bin/HttpServer --config=${WORKDIR}/../../examples/HttpDemo/HttpServer/config.conf &"

${WORKDIR}/../bin/HttpServer --config=${WORKDIR}/../../examples/HttpDemo/HttpServer/config.conf &

sleep 3

echo "client: ${WORKDIR}/../bin/HttpClient"

${WORKDIR}/../bin/HttpClient 2 10000

killall -9 HttpServer 


