#!/bin/bash

echo "run-quick-start.sh"

WORKDIR=$(pwd)

echo ${WORKDIR}

killall -9 QuickStartDemo

${WORKDIR}/../bin/QuickStartDemo --config=${WORKDIR}/../../examples/QuickStartDemo/HelloServer/Server/config.conf &

sleep 3

${WORKDIR}/../bin/QuickStartDemoClient


