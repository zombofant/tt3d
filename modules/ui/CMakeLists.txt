add_library(tt3d_ui STATIC "Surface.cpp" "Surface3x3.cpp" "Widget.cpp" "Boxes.cpp" "Button.cpp" "RootWidget.cpp")
add_dependencies(tt3d_ui tt3d_gl tt3d_math)
