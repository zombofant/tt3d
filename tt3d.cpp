#include <fstream>
#include "modules/io/Log.hpp"
#include "modules/core/UIBootstrap.hpp"

using namespace tt3d;

int main(int argc, const char *argv[]) {
    //IO::Log *log = new IO::Log();
    IO::OStreamHandle logStream = IO::OStreamHandle(new std::ofstream("log.txt"));
    IO::log.addLogTarget(new IO::LogOStreamTarget(logStream));
    
    IO::log << IO::ML_ERROR << "bla" << IO::submit;
    
    /*Core::TT3D *core = new Core::TT3D();
    core->runApp();
    delete core;*/
    return 0;
}
