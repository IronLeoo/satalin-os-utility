#!/bin/bash
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

gcc ${SCRIPT_DIR}/src/* -Wall -lstdc++ -lstdc++fs -g -o ${SCRIPT_DIR}/sosu