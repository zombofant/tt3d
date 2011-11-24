cmake_minimum_required(VERSION 2.6)
project(tt3d)

set(tt3d_VERSION_MAJOR 0)
set(tt3d_VERSION_MINOR 0)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
set(LIBS ${LIBS} ${OpenGL_LIBRARY})
add_executable(tt3d tt3d.cpp)
target_link_libraries(tt3d ${LIBS})
