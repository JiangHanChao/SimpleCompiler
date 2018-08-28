cmake_minimum_required (VERSION 4.0)

project (Compiler)

aux_source_directory(src DIR_SRCS)

include_directories(include)

add_executable(Compiler ${DIR_SRCS})