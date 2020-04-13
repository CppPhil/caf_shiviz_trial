#!/bin/bash

function catch_errors() {
    printf "\nrun_caf-vec.sh failed!\n" >&2
    exit 1
}

trap catch_errors ERR;

# Directory containing this bash script
readonly DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

readonly PREV_DIR=$(pwd)

readonly ACTOR_LOG_PATH=$DIR/actor_log.log

cd $DIR

cd ./build/caf/bin

./caf-vec --output-file=$DIR/caf-vec_output.log --include-hidden-actors --verbosity=2 $ACTOR_LOG_PATH

cd $PREV_DIR

exit 0

