#include "GenericBuffer.hpp"
#include <exception>
#include <stdexcept>
#include <cstddef>
#include <iostream>

namespace tt3d {
namespace GL {
    
using namespace tt3d;

/* tt3d::GL::GenericBuffer */

GenericBuffer::GenericBuffer(const GLsizei aItemSize, const GLenum aKind, const GLenum aPurpose):
    Class(),
    capacity(0),
    data(0),
    bufferPurpose(aPurpose),
    bufferKind(aKind),
    itemSize(aItemSize)
{
    
}

GenericBuffer::~GenericBuffer() {
    if (glID != 0) {
        freeBuffer();
    }
    if (data) {
        free(data);
    }
}

void GenericBuffer::doExpand(const GLsizei oldCapacity, const GLsizei newCapacity) {
    
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
    
    const GLsizei oldCapacity = capacity;
    capacity = newCapacity;
    doExpand(oldCapacity, newCapacity);
}

void GenericBuffer::flushRange(const GLsizei minItem, const GLsizei count) {
    if (glID == 0) {
        return;
    }
    bind();
    glBufferSubData(bufferKind, minItem * itemSize, count * itemSize, data);
}
    
void GenericBuffer::freeBuffer() {
    glDeleteBuffers(1, &glID);
    glID = 0;
}

void GenericBuffer::initBuffer() {
    glGenBuffers(1, &glID);
    std::cout << "initialized buffer " << glID << " capacity is currently " << capacity << std::endl;
    if (capacity > 0) {
        glBindBuffer(bufferKind, glID);
        glBufferData(bufferKind, capacity * itemSize, data, bufferPurpose);
    }
}

void GenericBuffer::rangeCheck(const GLsizei index) {
    if ((index < 0) || (index >= capacity)) {
        throw std::out_of_range("buffer index out of range");
    }
}

void GenericBuffer::requireBuffer() {
    if (glID == 0) {
        initBuffer();
    }
    if (glID == 0) {
        throw 0;
    }
}

void GenericBuffer::bind() {
    requireBuffer();
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
    requireBuffer();
    if (glID == 0) {
        return;
    }
    bind();
    glGetBufferSubData(bufferKind, 0, capacity * itemSize, data);
}

void GenericBuffer::unbind() {
    glBindBuffer(bufferKind, 0);
}


}
}
