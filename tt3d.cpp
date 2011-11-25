#include <GL/gl.h>
#include "modules/io/vfs/VFS.hpp"
#include "modules/io/vfs/MountDirectory.hpp"
#include "modules/geometry/Vectors.hpp"

int main(int argc, const char *argv[]) {
    using namespace tt3d::VFS;
    using namespace tt3d::Geometry;
    
    VFS vfs = VFS();
    MountPtr mount(new MountDirectory("/usr/local/share/tt3d"));
    vfs.addMount(mount, "/data/", MP_FILE_SYSTEM);
    mount = MountPtr(new MountDirectory("/home/horazont/Projects/c++/tt3d/"));
    vfs.addMount(mount, "/data/", MP_IMPORTANT);
    vfs.dumpMounts();
    std::cout << vfs.fileExists("/data/tt3d") << std::endl;
    
    Vector3 v1(1, 0, 0), v2(0, 1, 0), v3(0, 0, 1), v;
    v = v1 % v2;
    std::cout << v.x << " " << v.y << " " << v.z << std::endl;
    std::cout << v1 * v2 << std::endl;
    std::cout << (v1 + v2).normalized() * v2 << std::endl;
    return 0;
}
