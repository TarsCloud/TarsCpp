#!/bin/bash

echo "run-co.sh"

WORKDIR=$(pwd)

echo ${WORKDIR}

killall -9 PushServer 

echo "start server: ${WORKDIR}/../bin/PushServer --config=${WORKDIR}/../../examples/PushDemo/PushServer/config.conf &"

${WORKDIR}/../bin/PushServer --config=${WORKDIR}/../../examples/PushDemo/PushServer/config.conf &

sleep 3

#-------------------------------------------------------------------------------------------------------

echo "client: ${WORKDIR}/../bin/PushClient"

${WORKDIR}/../bin/PushClient 5



