cmake_minimum_required(VERSION 3.13)

project(FluideModelizer LANGUAGES C CXX)

find_package(SDL REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${SDL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})

set(ALL_LIBRARIES ${SDL_LIBRARY} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})

file(GLOB_RECURSE src_files    src/*.cpp)
file(GLOB_RECURSE header_files headers/*.hpp)

message(STATUS "source files founded : " ${src_files})
message(STATUS "header files founded : " ${header_files})

add_executable(modelizer1 ${src_files} ${header_files})
target_include_directories(modelizer1 PRIVATE ./headers)
target_link_libraries(modelizer1 ${ALL_LIBRARIES})