cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(CAF_ShiViz_Trial VERSION 1.0.0)

execute_process(
    COMMAND bash ${CAF_ShiViz_Trial_SOURCE_DIR}/scripts/build_caf.sh
)

set(CMAKE_MODULE_PATH ${CAF_ShiViz_Trial_SOURCE_DIR}/cmake)
set(CAF_ROOT_DIR ${CAF_ShiViz_Trial_SOURCE_DIR}/external/actor-framework/build)
find_package(CAF COMPONENTS core io test)

string(APPEND CMAKE_CXX_FLAGS " -Werror -Wall -Wextra -Wcast-align -Wcast-qual -Wmissing-field-initializers -Wmissing-include-dirs -Wpointer-arith -Wstack-protector -Wstrict-aliasing=2 -Wunreachable-code -Wlogical-op -Wduplicated-cond -Wnull-dereference -Wrestrict -Wno-unknown-pragmas -Wno-unused-function -Wno-deprecated-copy -Wno-pessimizing-move")
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -g -O0")
string(APPEND CMAKE_CXX_FLAGS_RELEASE " -O3 -march=native -funroll-loops -flto -fwhole-program -DNDEBUG")

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

find_package(Threads REQUIRED)

### OBJECT LIBRARY
set(LIB_NAME ShiViz_Trial_Lib)

set(LIB_HEADERS
   )
   
set(LIB_SOURCES
   )
   
add_library(
    ${LIB_NAME}
    OBJECT
    ${LIB_HEADERS}
    ${LIB_SOURCES}
)

target_link_libraries(
    ${LIB_NAME}
    PUBLIC
    Threads::Threads
)

target_link_libraries(
    ${LIB_NAME}
    PUBLIC
    ${CMAKE_DL_LIBS} ${CAF_LIBRARY_CORE} ${CAF_LIBRARY_IO}
)

target_include_directories(
    ${LIB_NAME}
    PUBLIC
    ${CAF_INCLUDE_DIRS}
)

target_include_directories(
    ${LIB_NAME}
    PUBLIC
    ${CAF_ShiViz_Trial_SOURCE_DIR}/include
)

### APPLICATION
set(APP_NAME ShiViz_Trial_App)

add_executable(
    ${APP_NAME}
    src/main.cpp
)

target_link_libraries(
    ${APP_NAME}
    PRIVATE
    ${LIB_NAME}
)
