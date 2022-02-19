cmake_minimum_required(VERSION 3.13)

project(FluideModelizer LANGUAGES C CXX)

find_package(SDL REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${SDL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} third-party)

set(ALL_LIBRARIES ${SDL_LIBRARY} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})

file(GLOB_RECURSE src_files    src/*.cpp)
file(GLOB_RECURSE header_files headers/*.hpp)
file(GLOB_RECURSE shader_files *.glsl)

message(STATUS "source files founded : " ${src_files})
message(STATUS "header files founded : " ${header_files})

add_executable(3Dgraphics ${src_files} ${header_files})
target_include_directories(3Dgraphics PRIVATE ./headers)
target_link_libraries(3Dgraphics ${ALL_LIBRARIES})

file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
