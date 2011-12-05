/**********************************************************************
File name: GeometryQuadsForTris.cpp
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
#include "GeometryQuadsForTris.hpp"

namespace tt3d {
namespace GL {
    
using namespace tt3d;

/* tt3d::GL::GeometryQuadsForTris */

VertexAllocationHandle GeometryQuadsForTris::allocateVertices() {
    VertexAllocation *allocation = new VertexAllocation(_material->getGeometryBuffer()->allocateVertices(_quadCount * 4));
    VertexIndexList *indicies = new VertexIndexList();
    indicies->reserve(_quadCount * 6);
    for (int i = 0; i < _quadCount; i++) {
        const int allocIdx = i * 4;
        indicies->push_back((*allocation->bufferAllocation)[allocIdx]);
        indicies->push_back((*allocation->bufferAllocation)[allocIdx+1]);
        indicies->push_back((*allocation->bufferAllocation)[allocIdx+2]);
        indicies->push_back((*allocation->bufferAllocation)[allocIdx]);
        indicies->push_back((*allocation->bufferAllocation)[allocIdx+2]);
        indicies->push_back((*allocation->bufferAllocation)[allocIdx+3]);
        
        /*indicies->push_back((*allocation->bufferAllocation)[allocIdx]);
        indicies->push_back((*allocation->bufferAllocation)[allocIdx+1]);
        indicies->push_back((*allocation->bufferAllocation)[allocIdx+2]);
        indicies->push_back((*allocation->bufferAllocation)[allocIdx+3]);*/
    }
    allocation->indicies = VertexIndexListHandle(indicies);
    allocation->staticHandle = _material->getStaticIndexBuffer()->add(allocation->indicies);
    return VertexAllocationHandle(allocation);
}

}
}
