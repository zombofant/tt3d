add_library(tt3d_terrain "generators/Source.cpp" "generators/PerlinNoiseSource.cpp")
add_dependencies(tt3d_terrain tt3d_geometry tt3d_gl)
