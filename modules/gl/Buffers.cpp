#include "Buffers.hpp"
#include <exception>
#include <stdexcept>

namespace tt3d {
namespace GL {
    
using namespace tt3d;

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

void GenericBuffer::flushRange(const GLsizei minItem, const GLsizei count) {
    if (glID == 0) {
        return;
    }
    bind();
    glBufferSubData(bufferKind, minItem * itemSize, count * itemSize, data);
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

}
}
