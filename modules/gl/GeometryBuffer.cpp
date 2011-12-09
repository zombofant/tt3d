/**********************************************************************
File name: GeometryBuffer.cpp
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
#include "GeometryBuffer.hpp"
#include <cassert>

namespace tt3d {
namespace GL {

/* tt3d::GL::VertexFormat */

VertexFormat::VertexFormat(const unsigned int aNPosition,
        const unsigned int aNColour,
        const unsigned int aNTexCoord0, 
        const unsigned int aNTexCoord1, 
        const unsigned int aNTexCoord2, 
        const unsigned int aNTexCoord3, 
        const bool aNormal, 
        const unsigned int aNVertexAttrib0, 
        const unsigned int aNVertexAttrib1, 
        const unsigned int aNVertexAttrib2, 
        const unsigned int aNVertexAttrib3):        
        nPosition(aNPosition),
        nColour(aNColour),
        nTexCoord0(aNTexCoord0),
        nTexCoord1(aNTexCoord1),
        nTexCoord2(aNTexCoord2),
        nTexCoord3(aNTexCoord3),
        nVertexAttrib0(aNVertexAttrib0),
        nVertexAttrib1(aNVertexAttrib1),
        nVertexAttrib2(aNVertexAttrib2),
        nVertexAttrib3(aNVertexAttrib3),
        normal(aNormal),
        posOffset(0),
        colourOffset(posOffset + nPosition * glTypeSize),
        texCoord0Offset(colourOffset + nColour * glTypeSize),
        texCoord1Offset(texCoord0Offset + nTexCoord0 * glTypeSize),
        texCoord2Offset(texCoord1Offset + nTexCoord1 * glTypeSize),
        texCoord3Offset(texCoord2Offset + nTexCoord2 * glTypeSize),
        normalOffset(texCoord3Offset + nTexCoord3 * glTypeSize),
        vertexAttrib0Offset(normalOffset + (normal?3:0) * glTypeSize),
        vertexAttrib1Offset(vertexAttrib0Offset + nVertexAttrib0 * glTypeSize),
        vertexAttrib2Offset(vertexAttrib1Offset + nVertexAttrib1 * glTypeSize),
        vertexAttrib3Offset(vertexAttrib2Offset + nVertexAttrib2 * glTypeSize),
        vertexSize(vertexAttrib3Offset + nVertexAttrib3 * glTypeSize)
{
    assert((nPosition >= 2) && (nPosition <= 4));
    assert((nColour >= 3) && (nColour <= 4));
    assert((nTexCoord0 <= 4));
    assert((nTexCoord1 <= 4));
    assert((nTexCoord2 <= 4));
    assert((nTexCoord3 <= 4));
    assert((nVertexAttrib0 <= 4));
    assert((nVertexAttrib1 <= 4));
    assert((nVertexAttrib2 <= 4));
    assert((nVertexAttrib3 <= 4));
    assert(vertexSize > 0); // this is a true assertion; with the first assert, this should never throw.
}

/* tt3d::GL::GenericGeometryBuffer */

GenericGeometryBuffer::GenericGeometryBuffer(const VertexFormatHandle vertexFormat, const GLenum aPurpose):
    GenericBuffer(vertexFormat->vertexSize, GL_ARRAY_BUFFER, aPurpose),
    _vertexFormat(vertexFormat)
{
    
}

void GenericGeometryBuffer::doExpand(const GLsizei oldCapacity, const GLsizei newCapacity)  
{
    GenericBuffer::doExpand(oldCapacity, newCapacity);
    for (VertexIndex i = oldCapacity; i < newCapacity; i++) {
        freeVertices.push_back(i);
    }
}

void GenericGeometryBuffer::gc_one(const VertexIndexListHandle handle) {
    VertexIndexList *list = handle.get();
    // freeVertices.splice(freeVertices.begin(), *list);
    for (VertexIndexList::iterator it = list->begin();
        it != list->end();
        it++)
    {
        freeVertices.push_back(*it);
    }
}

void GenericGeometryBuffer::get(const GLsizei index, const GLsizei offset, GLVertexFloat *value, const GLsizei n) {
    memcpy(value, data + (index * itemSize) + offset, n);    
}

GLsizei GenericGeometryBuffer::map(const GLsizei index) {
    if (bufferMap.get()) {
        return bufferMap->map(index);
    } else {
        return index;
    }
}

void GenericGeometryBuffer::set(const GLsizei index, const GLsizei offset, const GLVertexFloat *value, const GLsizei n) {
    memcpy(data + (index * itemSize) + offset, value, n);
    dirtyVertices.insert(index);
}

VertexIndexListHandle GenericGeometryBuffer::allocateVertices(const GLsizei count) {
    if ((GLsizei)freeVertices.size() < count) {
        gc();
        while ((GLsizei)freeVertices.size() < count) {
            expand();
        }
    }
    
    VertexIndexList *result = new VertexIndexList();
    for (GLsizei i = 0; i < count; i++) {
        result->push_back(freeVertices.front());
        freeVertices.pop_front();
    }
    
    VertexIndexListHandle handle = VertexIndexListHandle(result);
    handles.push_back(handle);
    return handle;
}

void GenericGeometryBuffer::gc() {
    bool changed = true;
    while (changed) {
        changed = false;
        for (VertexIndexListHandleList::iterator it = handles.begin();
            it != handles.end();
            it++)
        {
            VertexIndexListHandle handle = *it;
            if (handle.use_count() == 1) {
                gc_one(handle);
                handles.erase(it);
                changed = true;
                break;
            }
        }
    }
}

GeometryBufferDriverHandle GenericGeometryBuffer::createDriver(const VertexFormatHandle handle) 
{
    return createDriver(*handle);
}

Utils::BufferMapHandle GenericGeometryBuffer::getMap() 
{
    return bufferMap;
}

void GenericGeometryBuffer::flush() {
    GLsizei min, max;
    std::set<VertexIndex>::iterator it = dirtyVertices.begin();
    min = *it;
    max = *it;
    
    for (it++;
        it != dirtyVertices.end();
        it++) 
    {
        if (min > *it) {
            min = *it;
        }
        if (max < *it) {
            max = *it;
        }
    }
    flushRange(min, (max - min) + 1);
    dirtyVertices.clear();
}

void GenericGeometryBuffer::setMap(Utils::BufferMapHandle aValue) {
    bufferMap = aValue;
}

/* tt3d::GL::GeometryBufferDriver */

void GeometryBufferDriver::setPosition(const GLsizei index, const Vector2f &source)
{
    assert(source.dimension <= _vertexFormat->nPosition);
    _handle->set(index, _vertexFormat->posOffset, source.as_array, source.dimension);
}

void GeometryBufferDriver::setPosition(const GLsizei index, const Vector3f &source)
{
    assert(source.dimension <= _vertexFormat->nPosition);
    _handle->set(index, _vertexFormat->posOffset, source.as_array, source.dimension);
}

void GeometryBufferDriver::setPosition(const GLsizei index, const Vector4f &source)
{
    assert(source.dimension <= _vertexFormat->nPosition);
    _handle->set(index, _vertexFormat->posOffset, source.as_array, source.dimension);
}


void GeometryBufferDriver::setColour(const GLsizei index, const Vector3f &source)
{
    assert(source.dimension <= _vertexFormat->nColour);
    _handle->set(index, _vertexFormat->colourOffset, source.as_array, source.dimension);
}

void GeometryBufferDriver::setColour(const GLsizei index, const Vector4f &source)
{
    assert(source.dimension <= _vertexFormat->nColour);
    _handle->set(index, _vertexFormat->colourOffset, source.as_array, source.dimension);
}


void GeometryBufferDriver::setTexCoord0(const GLsizei index, const Vector2f &source)
{
    assert(source.dimension <= _vertexFormat->nTexCoord0);
    _handle->set(index, _vertexFormat->texCoord0Offset, source.as_array, source.dimension);
}

void GeometryBufferDriver::setTexCoord0(const GLsizei index, const Vector3f &source)
{
    assert(source.dimension <= _vertexFormat->nTexCoord0);
    _handle->set(index, _vertexFormat->texCoord0Offset, source.as_array, source.dimension);
}

void GeometryBufferDriver::setTexCoord0(const GLsizei index, const Vector4f &source)
{
    assert(source.dimension <= _vertexFormat->nTexCoord0);
    _handle->set(index, _vertexFormat->texCoord0Offset, source.as_array, source.dimension);
}


void GeometryBufferDriver::setTexCoord1(const GLsizei index, const Vector2f &source)
{
    assert(source.dimension <= _vertexFormat->nTexCoord1);
    _handle->set(index, _vertexFormat->texCoord1Offset, source.as_array, source.dimension);
}

void GeometryBufferDriver::setTexCoord1(const GLsizei index, const Vector3f &source)
{
    assert(source.dimension <= _vertexFormat->nTexCoord1);
    _handle->set(index, _vertexFormat->texCoord1Offset, source.as_array, source.dimension);
}

void GeometryBufferDriver::setTexCoord1(const GLsizei index, const Vector4f &source)
{
    assert(source.dimension <= _vertexFormat->nTexCoord1);
    _handle->set(index, _vertexFormat->texCoord1Offset, source.as_array, source.dimension);
}


void GeometryBufferDriver::setTexCoord2(const GLsizei index, const Vector2f &source)
{
    assert(source.dimension <= _vertexFormat->nTexCoord2);
    _handle->set(index, _vertexFormat->texCoord2Offset, source.as_array, source.dimension);
}

void GeometryBufferDriver::setTexCoord2(const GLsizei index, const Vector3f &source)
{
    assert(source.dimension <= _vertexFormat->nTexCoord2);
    _handle->set(index, _vertexFormat->texCoord2Offset, source.as_array, source.dimension);
}

void GeometryBufferDriver::setTexCoord2(const GLsizei index, const Vector4f &source)
{
    assert(source.dimension <= _vertexFormat->nTexCoord2);
    _handle->set(index, _vertexFormat->texCoord2Offset, source.as_array, source.dimension);
}


void GeometryBufferDriver::setTexCoord3(const GLsizei index, const Vector2f &source)
{
    assert(source.dimension <= _vertexFormat->nTexCoord3);
    _handle->set(index, _vertexFormat->texCoord3Offset, source.as_array, source.dimension);
}

void GeometryBufferDriver::setTexCoord3(const GLsizei index, const Vector3f &source)
{
    assert(source.dimension <= _vertexFormat->nTexCoord3);
    _handle->set(index, _vertexFormat->texCoord3Offset, source.as_array, source.dimension);
}

void GeometryBufferDriver::setTexCoord3(const GLsizei index, const Vector4f &source)
{
    assert(source.dimension <= _vertexFormat->nTexCoord3);
    _handle->set(index, _vertexFormat->texCoord3Offset, source.as_array, source.dimension);
}


void GeometryBufferDriver::setNormal(const GLsizei index, const Vector3f &source)
{
    assert(_vertexFormat->normal);
    _handle->set(index, _vertexFormat->normalOffset, source.as_array, source.dimension);
}


void GeometryBufferDriver::setVertexAttrib0(const GLsizei index, const Vector2f &source)
{
    assert(source.dimension <= _vertexFormat->nVertexAttrib0);
    _handle->set(index, _vertexFormat->vertexAttrib0Offset, source.as_array, source.dimension);
}

void GeometryBufferDriver::setVertexAttrib0(const GLsizei index, const Vector3f &source)
{
    assert(source.dimension <= _vertexFormat->nVertexAttrib0);
    _handle->set(index, _vertexFormat->vertexAttrib0Offset, source.as_array, source.dimension);
}

void GeometryBufferDriver::setVertexAttrib0(const GLsizei index, const Vector4f &source)
{
    assert(source.dimension <= _vertexFormat->nVertexAttrib0);
    _handle->set(index, _vertexFormat->vertexAttrib0Offset, source.as_array, source.dimension);
}


void GeometryBufferDriver::setVertexAttrib1(const GLsizei index, const Vector2f &source)
{
    assert(source.dimension <= _vertexFormat->nVertexAttrib1);
    _handle->set(index, _vertexFormat->vertexAttrib1Offset, source.as_array, source.dimension);
}

void GeometryBufferDriver::setVertexAttrib1(const GLsizei index, const Vector3f &source)
{
    assert(source.dimension <= _vertexFormat->nVertexAttrib1);
    _handle->set(index, _vertexFormat->vertexAttrib1Offset, source.as_array, source.dimension);
}

void GeometryBufferDriver::setVertexAttrib1(const GLsizei index, const Vector4f &source)
{
    assert(source.dimension <= _vertexFormat->nVertexAttrib1);
    _handle->set(index, _vertexFormat->vertexAttrib1Offset, source.as_array, source.dimension);
}


void GeometryBufferDriver::setVertexAttrib2(const GLsizei index, const Vector2f &source)
{
    assert(source.dimension <= _vertexFormat->nVertexAttrib2);
    _handle->set(index, _vertexFormat->vertexAttrib2Offset, source.as_array, source.dimension);
}

void GeometryBufferDriver::setVertexAttrib2(const GLsizei index, const Vector3f &source)
{
    assert(source.dimension <= _vertexFormat->nVertexAttrib2);
    _handle->set(index, _vertexFormat->vertexAttrib2Offset, source.as_array, source.dimension);
}

void GeometryBufferDriver::setVertexAttrib2(const GLsizei index, const Vector4f &source)
{
    assert(source.dimension <= _vertexFormat->nVertexAttrib2);
    _handle->set(index, _vertexFormat->vertexAttrib2Offset, source.as_array, source.dimension);
}


void GeometryBufferDriver::setVertexAttrib3(const GLsizei index, const Vector2f &source)
{
    assert(source.dimension <= _vertexFormat->nVertexAttrib3);
    _handle->set(index, _vertexFormat->vertexAttrib3Offset, source.as_array, source.dimension);
}

void GeometryBufferDriver::setVertexAttrib3(const GLsizei index, const Vector3f &source)
{
    assert(source.dimension <= _vertexFormat->nVertexAttrib3);
    _handle->set(index, _vertexFormat->vertexAttrib3Offset, source.as_array, source.dimension);
}

void GeometryBufferDriver::setVertexAttrib3(const GLsizei index, const Vector4f &source)
{
    assert(source.dimension <= _vertexFormat->nVertexAttrib3);
    _handle->set(index, _vertexFormat->vertexAttrib3Offset, source.as_array, source.dimension);
}

}
}
