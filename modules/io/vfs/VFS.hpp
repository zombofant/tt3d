#ifndef _MODULES_IO_VFS_HPP
#define _MODULES_IO_VFS_HPP

#include <string>
#include <vector>
#include <list>
#include <istream>
#include <ostream>
#include <iostream>
#include <boost/smart_ptr.hpp>

namespace tt3d {
namespace VFS {
    
enum MountPriority {
    MP_PRACTICALLY_INEXISTANT = -3,
    MP_DISCRIMINATED = -2,
    MP_FALLBACK = -1,
    MP_FILE_SYSTEM = 0,
    MP_IMPORTANT = 1,
    MP_OVERRIDE = 2,
    MP_PENETRANT = 3
};

enum WriteMode {
    WM_IGNORE = 0,
    WM_OVERWRITE = 1,
    WM_APPEND = 2
};

enum OpenMode {
    OM_READ = 0,
    OM_WRITE = 1,
    OM_BOTH = 2
};

enum ShareMode {
    SM_EXCLUSIVE = 0,
    SM_ALLOW_READ = 1,
    SM_ALLOW_WRITE = 2,
    SM_ALLOW_BOTH = 3,
    SM_DONT_CARE = 4
};

enum ProtocolCapability {
    PC_NONE = 0,
    PC_BIDIRECTIONAL = 1,
    PC_READ = 2,
    PC_WRITE = 4,
    PC_SHARE_MODES = 8
};
typedef unsigned int ProtocolCapabilities;

typedef boost::shared_ptr<std::ostream> OStreamHandle;
typedef boost::shared_ptr<std::istream> IStreamHandle;
typedef boost::shared_ptr<std::iostream> IOStreamHandle;

/** 
 * Checks whether a path leaves the root node.
 * 
 * @param path the path to check (preferable utf-8)
 * @return true if the path never leaves the root, false if any sequence 
 *         of .. is found which makes the path leave the scope.
 */
bool pathValid(const std::string path);

class Mount {
    protected:
        virtual void enableMount();
    public:
        virtual ProtocolCapabilities getCapabilities();
        virtual IOStreamHandle openBidirectional(std::string aPath, WriteMode writeMode = WM_IGNORE, ShareMode shareMode = SM_DONT_CARE) = 0;
        virtual IStreamHandle openReadStream(std::string aPath, ShareMode shareMode = SM_DONT_CARE);
        virtual OStreamHandle openWriteStream(std::string aPath, WriteMode writeMode = WM_IGNORE, ShareMode shareMode = SM_DONT_CARE);
        virtual bool fileExists(std::string aPath) = 0;
        
        virtual const std::string toString();
};

typedef boost::shared_ptr<Mount> MountPtr;

class MountWrapper {
    private:
        const std::string location;
        MountPtr mount;
        const MountPriority priority;
    public:
        MountWrapper (MountPtr aMount, const std::string aLocation, const MountPriority aPriority):
            location(aLocation),
            mount(aMount),
            priority(aPriority)
            {};
        
        const std::string getLocation();
        MountPtr getMount();
        MountPriority getPriority();
};
    
typedef boost::shared_ptr<MountWrapper> MountWrapperPtr;

bool comparePriority(MountWrapperPtr a, MountWrapperPtr b);

class VFS {
    private:
        std::list<MountWrapperPtr> *mounts;
    protected:
        MountPtr findMount(std::string aPath);
    public:
        VFS();
        
        void addMount(MountPtr aMount, const std::string aRoot, const MountPriority aPriority);
        std::iostream *openBidirectional(std::string aPath, WriteMode writeMode = WM_IGNORE, ShareMode shareMode = SM_DONT_CARE);
        std::istream *openReadStream(std::string aPath, ShareMode shareMode = SM_DONT_CARE);
        std::ostream *openWriteStream(std::string aPath, WriteMode writeMode = WM_IGNORE, ShareMode shareMode = SM_DONT_CARE);
        bool fileExists(std::string aPath);
        void dumpMounts();
};

}
}

#endif
