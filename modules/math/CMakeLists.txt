add_library(tt3d_math Vectors.cpp Matrices.cpp Rect.cpp Colours.cpp Utils.cpp Mesh.cpp)
add_dependencies(tt3d_math tt3d_utils)
