#!/bin/bash

# Directory containing this bash script
readonly DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

readonly PREVIOUS_DIRECTORY=$(pwd)

readonly ROOT_DIR=$DIR/..

readonly CAF_BUILD_DIR=$ROOT_DIR/build/caf

cd $ROOT_DIR/external

cd ./actor-framework

CC="ccache gcc" CXX="ccache g++" ./configure --generator="Unix Makefiles" --build-dir=$CAF_BUILD_DIR --build-type=Release --log-level=TRACE --enable-actor-profiler --disable-examples --disable-testing 
cd $CAF_BUILD_DIR
CC="ccache gcc" CXX="ccache g++" make -j$(nproc)

cd $PREVIOUS_DIRECTORY

exit 0

