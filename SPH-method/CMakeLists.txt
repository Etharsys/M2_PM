cmake_minimum_required(VERSION 3.13)

project(SPH LANGUAGES C CXX)

find_package(Eigen3 REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${Eigen_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})

set(ALL_LIBRARIES ${Eigen_LIBRARY} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

file(GLOB_RECURSE src_files src/*.cpp)
file(GLOB_RECURSE header_files headers/*.hpp)

message(STATUS "source files founded : " ${src_files})
message(STATUS "header files founded : " ${header_files})

add_executable(SPH ${src_files} ${header_files})
target_include_directories(SPH PRIVATE ./headers)
target_link_libraries(SPH ${ALL_LIBRARIES})