#!/bin/bash

# Directory containing this bash script
readonly DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

readonly PREVIOUS_DIRECTORY=$(pwd)

readonly ROOT_DIR=$DIR/..

cd $ROOT_DIR/external

cd ./actor-framework

./configure --build-type=Debug --no-examples --no-python --with-log-level=TRACE --with-actor-profiler --with-runtime-checks
cd build
make -j$(nproc)
cd ..

cd $PREVIOUS_DIRECTORY

exit 0

