/**********************************************************************
File name: Physical.hpp
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

#ifndef _TT3D_TRANSPORT_PHYSICAL
#define _TT3D_TRANSPORT_PHYSICAL

#include <vector>
#include "modules/math/Vector.hpp"

namespace tt3d {
namespace Transport {
    
enum PhysicalSide {
    PHYSICAL_SIDE_A = 0,
    PHYSICAl_SIDE_B = 1
};

class PhysicalLink {
    
};

class PhysicalNodeSide {
    
};

class PhysicalNodeDirection {
    public:
        PhysicalNodeDirection(PhysicalNode &owner,
            const Vector3 &tangent,
            const unsigned int index);
    private:
        const unsigned int _index;
        PhysicalNode &_owner;
        PhysicalNodeSide *_sides[2];
        const Vector3 _tangent;
    protected:
        void updateSides();
    public:
        PhysicalNode &getOwner() const { return _owner; };
        PhysicalNodeSide *getSide(const PhysicalSide side) const { return _sides[side]; };
        const Vector3 &getTangent() const { return _tangent; };
};

typedef std::vector<PhysicalNodeDirection> PhysicalNodeDirections;

class PhysicalNode;
typedef boost::shared_ptr<PhysicalNode> PhysicalNodeHandle;

class PhysicalNode {
    public:
        PhysicalNode(const Vector3 &location);
    private:
        const Vector3 _location;
        PhysicalNodeDirections _directions;
    protected:
        void fullClear();
    public:
        void clear();
        PhysicalNodeDirection &newDirection(const Vector3 &tangent);
    public:
        unsigned int getCount() const { return _directions.size(); };
        PhysicalNodeDirection &getDirection(const unsigned int index) { return _directions[index]; };
        const Vector3 &getLocation() const { return _location; };
};

}
}

#endif
