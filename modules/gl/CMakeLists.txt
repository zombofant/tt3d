add_library(tt3d_gl STATIC "Base.cpp" "Viewport.cpp" "Camera.cpp" "GenericBuffer.cpp" "GeometryBuffer.cpp" "IndexBuffer.cpp")
add_dependencies(tt3d_gl tt3d_geometry tt3d_utils)
