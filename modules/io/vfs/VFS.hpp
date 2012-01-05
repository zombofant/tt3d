/**********************************************************************
File name: VFS.hpp
This file is part of: tt3d — Freeform transport simulation

LICENSE

The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations under
the License.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public license (the  "GPL License"), in which case  the
provisions of GPL License are applicable instead of those above.

FEEDBACK & QUESTIONS

For feedback and questions about tt3d please e-mail one of the authors
named in the AUTHORS file.
**********************************************************************/
#ifndef _MODULES_IO_VFS_HPP
#define _MODULES_IO_VFS_HPP

#include <string>
#include <vector>
#include <list>
#include <istream>
#include <ostream>
#include <iostream>
#include <boost/smart_ptr.hpp>
#include "modules/io/FileStream.hpp"
#include "modules/utils/Exception.hpp"

namespace tt3d {    
namespace VFS {
    
using namespace IO;
using namespace Utils;

class VFSError: public Exception {
    public:
        VFSError(const std::string message): Exception(message) {};
        VFSError(const char* message): Exception(message) {};
        virtual ~VFSError() throw() {};
};

class VFSNotSupported: public VFSError {
    public:
        VFSNotSupported(const std::string message): VFSError(message) {};
        VFSNotSupported(const char* message): VFSError(message) {};
        virtual ~VFSNotSupported() throw() {};
};
    
enum MountPriority {
    MP_PRACTICALLY_INEXISTANT = -3,
    MP_DISCRIMINATED = -2,
    MP_FALLBACK = -1,
    MP_FILE_SYSTEM = 0,
    MP_IMPORTANT = 1,
    MP_OVERRIDE = 2,
    MP_PENETRANT = 3
};

enum ProtocolCapability {
    PC_NONE = 0,
    PC_BIDIRECTIONAL = 1,
    PC_READ = 2,
    PC_WRITE = 4,
    PC_SHARE_MODES = 8
};
typedef unsigned int ProtocolCapabilities;

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
        virtual StreamHandle openBidirectional(std::string aPath, WriteMode writeMode = WM_IGNORE, ShareMode shareMode = SM_DONT_CARE) = 0;
        virtual StreamHandle openReadStream(std::string aPath, ShareMode shareMode = SM_DONT_CARE);
        virtual StreamHandle openWriteStream(std::string aPath, WriteMode writeMode = WM_IGNORE, ShareMode shareMode = SM_DONT_CARE);
        virtual bool fileExists(std::string aPath) = 0;
        
        virtual const std::string toString();
};

typedef boost::shared_ptr<Mount> MountHandle;

class MountWrapper {
    public:
        MountWrapper(MountHandle aMount, const std::string aLocation, const MountPriority aPriority);
    private:
        const std::string _location;
        MountHandle _mount;
        const MountPriority _priority;
    public:
        const std::string getLocation() const;
        MountHandle getMount() const;
        MountPriority getPriority() const;
};
    
typedef boost::shared_ptr<MountWrapper> MountWrapperHandle;

bool comparePriority(MountWrapperHandle a, MountWrapperHandle b);

class VFS {
    private:
        std::list<MountWrapperHandle> *_mounts;
    protected:
        MountHandle findMount(std::string aPath);
    public:
        VFS();
        
        void addMount(MountHandle aMount, const std::string aRoot, const MountPriority aPriority);
        StreamHandle openBidirectional(std::string aPath, WriteMode writeMode = WM_IGNORE, ShareMode shareMode = SM_DONT_CARE);
        StreamHandle openReadStream(std::string aPath, ShareMode shareMode = SM_DONT_CARE);
        StreamHandle openWriteStream(std::string aPath, WriteMode writeMode = WM_IGNORE, ShareMode shareMode = SM_DONT_CARE);
        bool fileExists(std::string aPath);
        void dumpMounts();
};

}
}

#endif
