#ifndef _MODULES_IO_VFS_MOUNTDIRECTORY_HPP
#define _MODULES_IO_VFS_MOUNTDIRECTORY_HPP

#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <boost/smart_ptr.hpp>
#include "VFS.hpp"

namespace tt3d {
namespace VFS {

class MountDirectory: public Mount {
    private:
        const std::string rootPath;
    protected:
        std::ios_base::openmode writeModeToOpenMode(WriteMode writeMode);
    public:
        MountDirectory(const std::string aRootPath);
        
        ProtocolCapabilities getCapabilities();
        IOStreamHandle openBidirectional(std::string aPath, WriteMode writeMode = WM_IGNORE, ShareMode shareMode = SM_DONT_CARE);
        IStreamHandle openReadStream(std::string aPath, ShareMode shareMode = SM_DONT_CARE);
        OStreamHandle openWriteStream(std::string aPath, WriteMode writeMode = WM_IGNORE, ShareMode shareMode = SM_DONT_CARE);
        bool fileExists(std::string aPath);
        
        const std::string toString();
};

}
}
#endif
