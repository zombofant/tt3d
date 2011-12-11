add_library(tt3d_core STATIC "UIBootstrap.cpp" "InGame.cpp")
add_dependencies(tt3d_core tt3d_io tt3d_ui tt3d_terrain)
