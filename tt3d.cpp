#include "modules/core/UIBootstrap.hpp"
#include "modules/gl/Buffers.hpp"

using namespace tt3d;

int main(int argc, const char *argv[]) {
    Core::TT3D core = Core::TT3D();
    core.runApp();
    
    GL::GeometryBuffer<float, 3> test1(0);
    test1.getPosition(0, 0);
    return 0;
}
