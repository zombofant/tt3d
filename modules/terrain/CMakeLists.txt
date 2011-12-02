add_library(tt3d_terrain "generators/Source.cpp" "generators/PerlinNoiseSource.cpp" "TerrainMesh.cpp")
add_dependencies(tt3d_terrain tt3d_math tt3d_gl)
