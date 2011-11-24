#include "VFS.hpp"
#include <boost/smart_ptr.hpp>
#include <iostream>

namespace tt3d {
namespace VFS {

bool pathValid(std::string path) {
    bool hadSlash = true;
    unsigned char hadDot = 0;
    int levels = 0;
    for (std::string::iterator i = path.begin();
        i != path.end();
        i++) 
    {
        if (*i == '/') {
            if (hadDot == 2) {
                levels--;
                if (levels < 0) 
                    return false;
            } else if ((hadDot == 0) and (!hadSlash)) {
                levels++;
            }
            hadSlash = true;
            hadDot = 0;
        } else if ((hadSlash || (hadDot > 0)) && (*i == '.')) {
            hadDot++;
            hadSlash = false;
        } else {
            hadDot = 0;
            hadSlash = false;
        }
    }
    if (hadDot == 2) {
        levels--;
    }
    return levels >= 0;
}
    
/* tt3d::VFS::Mount */

void Mount::enableMount() {
    
}

ProtocolCapabilities Mount::getCapabilities() {
    return PC_NONE;
}

std::istream *Mount::openReadStream(std::string aPath, 
    ShareMode shareMode) 
{
    if (getCapabilities() & PC_READ) {
        return openBidirectional(aPath, WM_IGNORE, shareMode);
    } else {
        // TODO: Raise an exception here!
        return NULL;
    }
}

std::ostream *Mount::openWriteStream(std::string aPath,
    WriteMode writeMode, ShareMode shareMode)
{
    if (getCapabilities() & PC_WRITE) {
        return openBidirectional(aPath, writeMode, shareMode);
    } else {
        // TODO: Raise an exception here!
        return NULL;
    }
}

const std::string Mount::toString() {
    return "Unknown mount";
}

/* tt3d::VFS::MountWrapper */

const std::string MountWrapper::getLocation() {
    return location;
}

MountPtr MountWrapper::getMount() {
    return mount;
}

MountPriority MountWrapper::getPriority() {
    return priority;
}

bool comparePriority(MountWrapperPtr a, MountWrapperPtr b) {
    return a->getPriority() < b->getPriority();
}

/* tt3d::VFS::VFS */

VFS::VFS() {
    mounts = new std::list<MountWrapperPtr>;
}

void VFS::addMount(MountPtr aMount, const std::string aRoot, const MountPriority aPriority) {
    mounts->push_back(MountWrapperPtr(new MountWrapper(aMount, aRoot, aPriority)));
    mounts->sort(comparePriority);
}

void VFS::dumpMounts() {
    for (std::list<MountWrapperPtr>::iterator i = mounts->begin();
        i != mounts->end();
        i++) 
    {
        MountWrapperPtr p = *i;
        std::cout << "mount with priority " << p->getPriority() << " at \"" << p->getLocation() << "\": " << p->getMount()->toString() << std::endl;
    }
}

}
}
