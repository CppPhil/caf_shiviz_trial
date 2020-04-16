cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(HunterGate)

huntergate(URL "https://github.com/cpp-pm/hunter/archive/v0.23.249.tar.gz" SHA1
           "d45d77d8bba9da13e9290a180e0477e90accd89b")

project(CAF_ShiViz_Trial VERSION 1.0.0)

string(
  APPEND
  CMAKE_CXX_FLAGS
  " -Wall -Wextra -Wcast-align -Wcast-qual -Wmissing-field-initializers -Wmissing-include-dirs -Wpointer-arith -Wstack-protector -Wstrict-aliasing=2 -Wunreachable-code -Wlogical-op -Wduplicated-cond -Wnull-dereference -Wno-unused-parameter"
)
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -g -O0")
string(APPEND CMAKE_CXX_FLAGS_RELEASE
       " -O3 -march=native -funroll-loops -flto -DNDEBUG")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

find_package(Threads REQUIRED)

set(CAF_ENABLE_ACTOR_PROFILER
    ON
    CACHE BOOL "Enable experimental profiler API" FORCE)
set(CAF_ENABLE_EXAMPLES
    OFF
    CACHE BOOL "Build small programs showcasing CAF features" FORCE)
set(CAF_ENABLE_TESTING
    OFF
    CACHE BOOL "Build unit test suites" FORCE)
set(CAF_LOG_LEVEL
    "TRACE"
    CACHE STRING "Set log verbosity of CAF components" FORCE)
add_subdirectory(external/actor-framework)

set(BUILD_TESTING
    OFF
    CACHE BOOL "Build jaeger tests" FORCE)
set(JAEGERTRACING_BUILD_EXAMPLES
    OFF
    CACHE BOOL "Build examples" FORCE)
add_subdirectory(external/jaeger-client-cpp)

# OBJECT LIBRARY
set(LIB_NAME ShiViz_Trial_Lib)

set(LIB_HEADERS
    include/args.hpp
    include/create_span.hpp
    include/error.hpp
    include/extract.hpp
    include/inject.hpp
    include/setup_tracer.hpp
    include/source_location.hpp
    include/test_profiler.hpp
    include/test_tracing_data.hpp
    include/test_tracing_data_factory.hpp
    include/upper.hpp)

set(LIB_SOURCES
    src/args.cpp
    src/create_span.cpp
    src/error.cpp
    src/extract.cpp
    src/inject.cpp
    src/setup_tracer.cpp
    src/test_profiler.cpp
    src/test_tracing_data.cpp
    src/test_tracing_data_factory.cpp
    src/upper.cpp)

add_library(${LIB_NAME} OBJECT ${LIB_HEADERS} ${LIB_SOURCES})

target_link_libraries(${LIB_NAME} PUBLIC Threads::Threads jaegertracing
                                         ${CMAKE_DL_LIBS} CAF::core CAF::io)

target_include_directories(
  ${LIB_NAME}
  PUBLIC ${CAF_INCLUDE_DIRS}
         ${CAF_ShiViz_Trial_SOURCE_DIR}/include
         ${CAF_ShiViz_Trial_SOURCE_DIR}/external/expected/include
         ${CAF_ShiViz_Trial_SOURCE_DIR}/external/optional/include
         ${CAF_ShiViz_Trial_SOURCE_DIR}/external/philslib/include)

target_compile_definitions(${LIB_NAME} PUBLIC CAF_ENABLE_ACTOR_PROFILER)

option(VERBOSE_SPANS "Enable verbose spans" OFF)
if(VERBOSE_SPANS)
  target_compile_definitions(${LIB_NAME} PUBLIC VERBOSE_SPANS)
endif()

# APPLICATION
set(APP_NAME ShiViz_Trial_App)

add_executable(${APP_NAME} src/main.cpp)

target_link_libraries(${APP_NAME} PRIVATE ${LIB_NAME})
