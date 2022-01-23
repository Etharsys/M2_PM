cmake_minimum_required(VERSION 3.13)

project(M2PM LANGUAGES C CXX)

find_package(SDL REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

set(LIBRARY_FOLDER_NAME "M2PM")
set(SRC_FOLDER_NAME "src")
set(INCLUDE_FOLDER_NAME "include")

file(GLOB_RECURSE SRC_FILES ${LIBRARY_FOLDER_NAME}/${SRC_FOLDER_NAME}/*.cpp ${LIBRARY_FOLDER_NAME}/${INCLUDE_FOLDER_NAME}/*.hpp)

add_library(${LIBRARY_FOLDER_NAME} ${SRC_FILES})

include_directories(${SDL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${LIBRARY_FOLDER_NAME}/${INCLUDE_FOLDER_NAME})

set(ALL_LIBRARIES ${LIBRARY_FOLDER_NAME} ${SDL_LIBRARY} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})

file(GLOB_RECURSE SRC_FILES ${SRC_FOLDER_NAME}/*.cpp)
file(GLOB_RECURSE shader_files *.glsl)

message(STATUS "source files founded : " ${SRC_FILES})

foreach(MAIN_FILE ${SRC_FILES})
    get_filename_component(FILE ${MAIN_FILE} NAME_WE)
    add_executable(${FILE} ${MAIN_FILE})
    target_link_libraries(${FILE} ${ALL_LIBRARIES})
endforeach()


file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})