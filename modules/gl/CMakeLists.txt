add_library(tt3d_gl STATIC "Base.cpp" "Viewport.cpp" "Camera.cpp" "GenericBuffer.cpp" "GeometryBuffer.cpp" "IndexBuffer.cpp" "Material.cpp" "GeometryObject.cpp" "GeometryQuadsForTris.cpp")
add_dependencies(tt3d_gl tt3d_io tt3d_math tt3d_utils)
