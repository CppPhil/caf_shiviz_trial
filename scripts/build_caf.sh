#!/bin/bash

# Directory containing this bash script
readonly DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

readonly PREVIOUS_DIRECTORY=$(pwd)

readonly ROOT_DIR=$DIR/..

cd $ROOT_DIR/external

cd ./actor-framework

cp ./CMakeLists.txt ./CMakeLists.txt.backup
rm -f ./CMakeLists.txt
cp $ROOT_DIR/CustomCafCMakeLists.txt ./CMakeLists.txt

./configure --libs-only --with-actor-profiler

cd ./build

make -j$(nproc)

mv $ROOT_DIR/external/actor-framework/CMakeLists.txt.backup $ROOT_DIR/external/actor-framework/CMakeLists.txt

cd $PREVIOUS_DIRECTORY

exit 0

