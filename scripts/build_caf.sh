#!/bin/bash

# Directory containing this bash script
readonly DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

readonly PREVIOUS_DIRECTORY=$(pwd)

readonly ROOT_DIR=$DIR/..

cd $ROOT_DIR/external

cd ./actor-framework

if [ ! -d ./build ] || [ "$1" == "rebuild" ]; then
    rm -rf ./build
    
    git am < $ROOT_DIR/fix_tracing_data_factory.patch

    ./configure --no-examples --no-python --no-unit-tests --with-log-level=TRACE --with-actor-profiler

    cd ./build

    make -j$(nproc)

    cd ..

    git reset --hard HEAD~1
fi

cd $PREVIOUS_DIRECTORY

exit 0

