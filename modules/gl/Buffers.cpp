#include "Buffers.hpp"
#include <exception>
#include <stdexcept>
#include <cstddef>

namespace tt3d {
namespace GL {
    
using namespace tt3d;

/* tt3d::GL::IndexEntry */

IndexEntry::IndexEntry(const VertexIndex aStart, const VertexIndexListHandle aVertices):
    start(aStart),
    vertices(aVertices),
    count(aVertices->size())
{
    
}

/* tt3d::GL::GenericBuffer */

GenericBuffer::GenericBuffer(const GLsizei aItemSize, const GLenum aKind, const GLenum aPurpose):
    bufferPurpose(aPurpose),
    bufferKind(aKind),
    itemSize(aItemSize)
{
    initBuffer();
}

GenericBuffer::~GenericBuffer() {
    if (glID != 0) {
        freeBuffer();
    }
}

void GenericBuffer::expand() {
    const GLsizei newCapacity = (capacity ? capacity * 2 : 128);
    const GLsizei oldSize = capacity * itemSize;
    const GLsizei newSize = newCapacity * itemSize;
    
    data = (unsigned char*)realloc(data, newSize);
    
    if (glID != 0) {
        glBindBuffer(bufferKind, glID);
        glBufferData(bufferKind, newSize, 0, bufferPurpose);
        glBufferSubData(bufferKind, 0, oldSize, data);
    }
}

void GenericBuffer::flushRange(const GLsizei minItem, const GLsizei count) {
    if (glID == 0) {
        return;
    }
    bind();
    glBufferSubData(bufferKind, minItem * itemSize, count * itemSize, data);
}

void GenericBuffer::rangeCheck(const GLsizei index) {
    if ((index < 0) || (index >= capacity)) {
        throw std::out_of_range("buffer index out of range");
    }
}

void GenericBuffer::bind() {
    glBindBuffer(bufferKind, glID);
}

void GenericBuffer::flush() {
    if (glID == 0) {
        return;
    }
    bind();
    glBufferSubData(bufferKind, 0, capacity * itemSize, data);
}

void GenericBuffer::readBack() {
    if (glID == 0) {
        return;
    }
    bind();
    glGetBufferSubData(bufferKind, 0, capacity * itemSize, data);
}

void GenericBuffer::unbind() {
    glBindBuffer(bufferKind, 0);
}

/* tt3d::GL::GenericIndexBuffer */

GenericIndexBuffer::GenericIndexBuffer(const GLenum aPurpose):
    GenericBuffer(sizeof(GLuint), GL_ELEMENT_ARRAY_BUFFER, aPurpose),
    count(0)
{
    
}

void GenericIndexBuffer::clear() {
    count = 0;
}

void GenericIndexBuffer::draw(const GLenum mode) {
    glDrawElements(mode, count, GL_UNSIGNED_INT, 0);
}

/* tt3d::GL::StreamIndexBuffer */

StreamIndexBuffer::StreamIndexBuffer(const GLenum aPurpose):
    GenericIndexBuffer(aPurpose)
{
    
}

void StreamIndexBuffer::add(const VertexIndexListHandle vertices) {
    VertexIndexList *list = vertices.get();
    const GLsizei len = (GLsizei)list->size();
    while (count + len > capacity) {
        expand();
    }
    GLuint *dataptr = (GLuint *)data;
    dataptr += count;
    for (VertexIndexList::iterator it = list->begin();
        it != list->end();
        it++)
    {
        *dataptr = *it;
        dataptr++;
    }
}

/* tt3d::GL::StaticIndexBuffer */

StaticIndexBuffer::StaticIndexBuffer(const GLenum aPurpose):
    GenericIndexBuffer(aPurpose),
    handles(new IndexEntryHandles())
{
    
}

void StaticIndexBuffer::compress() {
    VertexIndex minChange = -1, maxChange = count;
    VertexIndex nextStart = 0;
    for (IndexEntryHandles::iterator it = handles->begin();
        it != handles->end();
        it++)
    {
        const IndexEntryHandle handle = *it;
        IndexEntry *entry = handle.get();
        
        if (entry->start > nextStart) {
            if ((minChange > nextStart) || (minChange == -1)) {
                minChange = nextStart;
            }
            updateHandles(it, (entry->start - nextStart));
        }
        
        nextStart = entry->start + entry->count;
    }
    count = nextStart;
    if ((minChange < maxChange) && (minChange >= 0)) {
        flushRange(minChange, (maxChange - minChange) + 1);
    }
}

void StaticIndexBuffer::updateHandles(const IndexEntryHandles::iterator startAt, const GLsizei offset) {
    for (IndexEntryHandles::iterator it(startAt);
        it != handles->end();
        it++)
    {
        const IndexEntryHandle handle = *it;
        IndexEntry *entry = handle.get();
        entry->start += offset;
    }
}

const IndexEntryHandle StaticIndexBuffer::add(const VertexIndexListHandle vertices) {
    VertexIndexList *list = vertices.get();
    const GLsizei addCount = (GLsizei)vertices->size();
    
    
    if (count + addCount > capacity) {
        gc();
        while (count + addCount > capacity) {
            expand();
        }
    }
    
    const IndexEntryHandle handle(new IndexEntry(count, vertices));
    IndexEntry *entry = handle.get();
    
    GLuint *dataptr = (GLuint *)data;
    dataptr += entry->start;
    
    for (VertexIndexList::iterator it = list->begin();
        it != list->end();
        it++)
    {
        *dataptr = *it;
        dataptr++;
    }
    count += entry->count;
    
    handles->push_back(handle);
    
    return handle;
}

void StaticIndexBuffer::clear() {
    GenericIndexBuffer::clear();
    handles->clear();
}

void StaticIndexBuffer::gc() {
    bool changed = true;
    bool globalChanged = false;
    while (changed) {
        changed = false;
        for (IndexEntryHandles::iterator it = handles->begin();
            it != handles->end();
            it++)
        {
            const IndexEntryHandle handle = *it;
            if (handle.use_count() == 1) {
                remove(handle, false);
                changed = true;
                globalChanged = true;
                break;
            }
        }
    }
    if (globalChanged) {
        compress();
    }
}

void StaticIndexBuffer::remove(const IndexEntryHandle handle, const bool autoCompress) {
    IndexEntryHandles::iterator it = find(handles->begin(), handles->end(), handle);
    if (it == handles->end()) {
        return;
    }
    
    handles->erase(it);
    
    // We must not change count here, as for now we allow removing 
    // handles without deallocating their space (see autoCompress)
    if (autoCompress) {
        compress();
    }
}

}
}
