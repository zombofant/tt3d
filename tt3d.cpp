#include <GL/gl.h>
#include "modules/io/vfs/VFS.hpp"
#include "modules/io/vfs/MountDirectory.hpp"

int main(int argc, const char *argv[]) {
    using namespace tt3d::VFS;
    
    VFS vfs = VFS();
    MountPtr mount(new MountDirectory("/usr/local/share/tt3d"));
    vfs.addMount(mount, "/data/", MP_FILE_SYSTEM);
    vfs.dumpMounts();
    return 0;
}
