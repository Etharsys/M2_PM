cmake_minimum_required(VERSION 3.13)

project(FluideModelizer LANGUAGES C CXX)

file(GLOB_RECURSE src_files    src/*.cpp)
file(GLOB_RECURSE header_files headers/*.hpp)

message(STATUS "source files founded : " ${src_files})
message(STATUS "header files founded : " ${header_files})

add_executable(modelizer1 ${src_files} ${header_files})
target_include_directories(modelizer1 PRIVATE ./headers)