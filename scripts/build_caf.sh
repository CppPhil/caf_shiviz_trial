#!/bin/bash

# Directory containing this bash script
readonly DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

readonly PREVIOUS_DIRECTORY=$(pwd)

readonly ROOT_DIR=$DIR/..

cd $ROOT_DIR/external

cd ./actor-framework

./configure --libs-only --with-actor-profiler

cd ./build

make -j$(nproc)

cd $PREVIOUS_DIRECTORY

exit 0

