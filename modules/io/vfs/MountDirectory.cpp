#include "MountDirectory.hpp"
#include <fstream>
#include <unistd.h>

namespace tt3d {
namespace VFS {

/* tt3d::VFS::MountDirectory */

MountDirectory::MountDirectory(const std::string aRootPath):
    rootPath(aRootPath) 
{
    
}

std::ios_base::openmode MountDirectory::writeModeToOpenMode(WriteMode writeMode) {
    switch (writeMode) {
        case WM_OVERWRITE:
        case WM_IGNORE: return (std::ios_base::out | std::ios_base::trunc);
        
        case WM_APPEND: return (std::ios_base::out | std::ios_base::app);
        
        default: return (std::ios_base::out | std::ios_base::trunc);
    }
}

ProtocolCapabilities MountDirectory::getCapabilities() {
    return PC_READ + PC_WRITE;
}

IOStreamHandle MountDirectory::openBidirectional(std::string aPath, WriteMode writeMode, ShareMode shareMode) {
    return IOStreamHandle();
}

IStreamHandle MountDirectory::openReadStream(std::string aPath, ShareMode shareMode) {
    if (!pathValid(aPath)) {
        return IStreamHandle();
    }
    std::string fullPath = rootPath + aPath;
    return IStreamHandle(new std::ifstream(fullPath.c_str()));
}

OStreamHandle MountDirectory::openWriteStream(std::string aPath, WriteMode writeMode, ShareMode shareMode) {
    if (!pathValid(aPath)) {
        return OStreamHandle();
    }
    std::string fullPath = rootPath + aPath;
    return OStreamHandle(new std::ofstream(fullPath.c_str(), writeModeToOpenMode(writeMode)));
}

bool MountDirectory::fileExists(std::string aPath) {
    if (!pathValid(aPath))
        return false;
    std::string fullPath = rootPath + aPath;
    // TODO: Check whether this works on non *nix
    return access(fullPath.c_str(), F_OK) != -1;
}

const std::string MountDirectory::toString() {
    return "directory \"" + rootPath + "\"";
}

}
}
