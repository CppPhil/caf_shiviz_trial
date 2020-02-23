#!/bin/bash

# Directory containing this bash script
readonly DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

readonly PREVIOUS_DIRECTORY=$(pwd)

readonly ROOT_DIR=$DIR/..

cd $ROOT_DIR/external

cd ./actor-framework

git am < $ROOT_DIR/fix_tracing_data_factory.patch

./configure --libs-only --with-actor-profiler

cd ./build

make -j$(nproc)

git reset --hard HEAD~1

cd $PREVIOUS_DIRECTORY

exit 0

