add_library(tt3d_ui STATIC "Widget.cpp" "Boxes.cpp" "Button.cpp")
add_dependencies(tt3d_ui tt3d_gl tt3d_geometry)
