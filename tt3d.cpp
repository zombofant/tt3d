#include "modules/core/UIBootstrap.hpp"

using namespace tt3d;

int main(int argc, const char *argv[]) {
    Core::TT3D *core = new Core::TT3D();
    core->runApp();
    delete core;
    return 0;
}
