add_library(tt3d_gl STATIC "Base.cpp" "Viewport.cpp" "Camera.cpp" "Buffers.cpp")
add_dependencies(tt3d_gl tt3d_geometry)
