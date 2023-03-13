#!/bin/bash

if [ "$1" = "debug" ]; then
    debug="-g"
else
    debug=""
fi

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

gcc ${SCRIPT_DIR}/src/* -Wall -lstdc++ -lstdc++fs -ljsoncpp $debug -o ${SCRIPT_DIR}/sosu