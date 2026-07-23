#!/usr/bin/env bash

if [[ $# -lt 3 || $# -gt 4 ]]; then
        echo "usage: $0 ProcessNum ThreadNum Size [sync|async]" >&2
        exit 1
fi

mode="${4:-sync}"
if [[ "$mode" != "sync" && "$mode" != "async" ]]; then
        echo "CallMode must be sync or async" >&2
        exit 1
fi

for((i=1; i<=$1; i++)); do {
        ./TarsStressClient "$2" 1500000 Test.StressServer.StressObj "$3" "$mode"
} & done
wait
