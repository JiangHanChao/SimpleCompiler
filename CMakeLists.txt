cmake_minimum_required (VERSION 2.8)

project (Compiler)

aux_source_directory(./src/ DIR_SRCS)

include_directories(./include/)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./build/)

add_executable(Compiler ${DIR_SRCS})

add_definitions(-std=c++11)

add_executable(spm spm.cpp)
