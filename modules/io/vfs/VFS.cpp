/**********************************************************************
File name: VFS.cpp
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

StreamHandle Mount::openReadStream(std::string aPath, 
    ShareMode shareMode) 
{
    if (getCapabilities() & PC_READ) {
        return openBidirectional(aPath, WM_IGNORE, shareMode);
    } else {
        throw VFSNotSupported("Reading from a non-readable mount.");
    }
}

StreamHandle Mount::openWriteStream(std::string aPath,
    WriteMode writeMode, ShareMode shareMode)
{
    if (getCapabilities() & PC_WRITE) {
        return openBidirectional(aPath, writeMode, shareMode);
    } else {
        throw VFSNotSupported("Writing to a non-writable mount.");
    }
}

const std::string Mount::toString() {
    return "Unknown mount";
}

/* tt3d::VFS::MountWrapper */

MountWrapper::MountWrapper (MountHandle aMount, const std::string aLocation, const MountPriority aPriority):
    _location(aLocation),
    _mount(aMount),
    _priority(aPriority)
{
    
};

const std::string MountWrapper::getLocation() const {
    return _location;
}

MountHandle MountWrapper::getMount() const {
    return _mount;
}

MountPriority MountWrapper::getPriority() const {
    return _priority;
}

bool comparePriority(MountWrapperHandle a, MountWrapperHandle b) {
    int cmp = b->getPriority() - a->getPriority();
    if (cmp == 0) {
        return a->getLocation().length() > b->getLocation().length();
    } else {
        return cmp < 0;
    }
}

/* tt3d::VFS::VFS */

VFS::VFS():
    _mounts(new std::list<MountWrapperHandle>)
{
    
}

MountHandle VFS::findMount(std::string path) {
    for (std::list<MountWrapperHandle>::iterator i = _mounts->begin();
        i != _mounts->end();
        i++)
    {
        MountWrapperHandle p = *i;
        const std::string location = p->getLocation();
        if (location.compare(0, location.length(), path, 0, location.length()) == 0) {
            std::string stripped = path.substr(location.length());
            MountHandle result = p->getMount();
            if (result->fileExists(stripped)) {
                return result;
            }
        }
    }
    return MountHandle();
}

void VFS::addMount(MountHandle aMount, const std::string aRoot, const MountPriority aPriority) {
    _mounts->push_back(MountWrapperHandle(new MountWrapper(aMount, aRoot, aPriority)));
    // FIXME: Can we make the mount container somehow sort itself automagically?
    _mounts->sort(comparePriority);
}

void VFS::dumpMounts() {
    for (std::list<MountWrapperHandle>::iterator i = _mounts->begin();
        i != _mounts->end();
        i++) 
    {
        MountWrapperHandle p = *i;
        std::cout << "mount with priority " << p->getPriority() << " at \"" << p->getLocation() << "\": " << p->getMount()->toString() << std::endl;
    }
}

bool VFS::fileExists(std::string path) {
    return (findMount(path) != MountHandle());
}

}
}
