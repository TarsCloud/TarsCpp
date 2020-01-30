#!/bin/bash

echo "run-quick-start.sh"

WORKDIR=$(pwd)

echo ${WORKDIR}

killall -9 QuickStartDemo

echo "start server: ${WORKDIR}/../bin/QuickStartDemo --config=${WORKDIR}/../../examples/QuickStartDemo/HelloServer/Server/config.conf &"

${WORKDIR}/../bin/QuickStartDemo --config=${WORKDIR}/../../examples/QuickStartDemo/HelloServer/Server/config.conf &

sleep 3

echo "client: ${WORKDIR}/../bin/QuickStartDemoClient"

${WORKDIR}/../bin/QuickStartDemoClient


