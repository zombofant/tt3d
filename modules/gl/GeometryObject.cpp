/**********************************************************************
File name: GeometryObject.cpp
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
#include "GeometryObject.hpp"

namespace tt3d {
namespace GL {
    
using namespace tt3d;

/* tt3d::GL::VertexIndexListMap */

VertexIndexListMap::VertexIndexListMap(const VertexIndexListHandle vertices):
    Utils::BufferMap(),
    _vertices(vertices)
{
    
}

void VertexIndexListMap::rangeCheck(const size_t index) {
    if ((index < 0) || (index >= _vertices->size())) {
        throw 0;
    }
}

size_t VertexIndexListMap::map(const size_t index) {
    const size_t mappedIndex = Utils::BufferMap::map(index);
    rangeCheck(mappedIndex);
    const VertexIndexList *list = _vertices.get();
    return (*list)[mappedIndex];
}

/* tt3d::GL::GeometryObject */

GeometryObject::GeometryObject(const MaterialHandle material):
    _material(material)
{
    
}

void GeometryObject::requireAllocation() {
    if (!_allocation.get())
        _allocation = allocateVertices();
}

void GeometryObject::addToStreamBuffer() {
    addToStreamBuffer(_material->getStreamIndexBuffer());
}

void GeometryObject::addToStreamBuffer(StreamIndexBufferHandle indexBuffer) {
    requireAllocation();
    indexBuffer->add(_allocation->indicies);
}

void GeometryObject::drawDirect(const GLenum mode) {
    requireAllocation();
    _material->getGeometryBuffer()->bind();
    _material->getStaticIndexBuffer()->drawHandle(_allocation->staticHandle, mode);
    _material->getGeometryBuffer()->unbind();
}

Utils::BufferMapHandle GeometryObject::getMap() {
    requireAllocation();
    return Utils::BufferMapHandle(new VertexIndexListMap(_allocation->bufferAllocation));
}

/* tt3d::GL::GeometryRaw */

GeometryRaw::GeometryRaw(const MaterialHandle material, const GLsizei vertexCount):
    GeometryObject(material),
    _vertexCount(vertexCount)
{
    
}

VertexAllocationHandle GeometryRaw::allocateVertices() {
    VertexAllocation *allocation = new VertexAllocation(_material->getGeometryBuffer()->allocateVertices(_vertexCount));
    allocation->indicies = VertexIndexListHandle(new VertexIndexList(*allocation->bufferAllocation.get()));
    allocation->staticHandle = _material->getStaticIndexBuffer()->add(allocation->indicies);
    return VertexAllocationHandle(allocation);
}

}
}
