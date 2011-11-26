cmake_minimum_required(VERSION 2.6)
project(tt3d CXX)

set(CMAKE_CXX_FLAGS "-g -Wall -pedantic")

set(tt3d_VERSION_MAJOR 0)
set(tt3d_VERSION_MINOR 0)
set(tt3d_VERSION_RELEASE 0)

# find_package(rt REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Boost COMPONENTS thread regex program_options system signals REQUIRED)
find_package(SDL REQUIRED)
# find_package(GLEW REQUIRED)
# TODO: proper packages for GLEW and rt

include_directories(${PROJECT_SOURCE_DIR} ${OpenGL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
set(LIBS ${LIBS} ${OpenGL_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_SIGNALS_LIBRARY} ${SDL_LIBRARY} rt GLU GLEW)

add_subdirectory("modules")

# why do we need to put the libraries in reversed order?
set(tt3d_MODULES tt3d_core tt3d_gl tt3d_geometry tt3d_io)

add_executable(tt3d tt3d.cpp)
add_dependencies(tt3d ${tt3d_MODULES})
target_link_libraries(tt3d ${tt3d_MODULES} ${LIBS})
