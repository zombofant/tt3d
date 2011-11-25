cmake_minimum_required(VERSION 2.6)
project(tt3d CXX)

set(tt3d_VERSION_MAJOR 0)
set(tt3d_VERSION_MINOR 0)
set(tt3d_VERSION_RELEASE 0)

find_package(OpenGL REQUIRED)
find_package(Boost COMPONENTS thread regex program_options system signals REQUIRED)
find_package(SDL REQUIRED)

include_directories(${OpenGL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
set(LIBS ${LIBS} ${OpenGL_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_SIGNALS_LIBRARY} ${SDL_LIBRARY})

add_subdirectory("modules")

add_executable(tt3d tt3d.cpp)
add_dependencies(tt3d tt3d_io tt3d_geometry)
target_link_libraries(tt3d ${LIBS} tt3d_io tt3d_geometry)
