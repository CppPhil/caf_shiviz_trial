cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(CAF_ShiViz_Trial VERSION 1.0.0)

execute_process(
    COMMAND bash ${CAF_ShiViz_Trial_SOURCE_DIR}/scripts/build_caf.sh
)

set(CMAKE_MODULE_PATH ${CAF_ShiViz_Trial_SOURCE_DIR}/cmake)
set(CAF_ROOT_DIR ${CAF_ShiViz_Trial_SOURCE_DIR}/external/actor-framework/build)
find_package(CAF COMPONENTS core io test)


