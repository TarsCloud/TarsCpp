#!/bin/bash

echo "run-quick-start.sh"

WORKDIR=$(pwd)

echo ${WORKDIR}

killall -9 QuickStartDemo

echo "${WORKDIR}/../bin/QuickStartDemo --config=${WORKDIR}/../../examples/QuickStartDemo/HelloServer/Server/config.conf"
${WORKDIR}/../bin/QuickStartDemo --config=${WORKDIR}/../../examples/QuickStartDemo/HelloServer/Server/config.conf &

sleep 3

echo "${WORKDIR}/../bin/QuickStartDemoClient"
${WORKDIR}/../bin/QuickStartDemoClient


