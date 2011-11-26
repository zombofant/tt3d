#ifndef _TT3D_GL_GENERIC_BUFFERS_H
#define _TT3D_GL_GENERIC_BUFFERS_H

#include <GL/glew.h>
#include "Base.hpp"
#include <limits>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <list>
#include <set>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "modules/utils/BufferMap.hpp"

namespace tt3d {
namespace GL {
    
using namespace tt3d;

class GenericBuffer: public Class {
    public:
        GenericBuffer(const GLsizei aItemSize, const GLenum aKind, const GLenum aPurpose);
        virtual ~GenericBuffer();
    protected:
        GLsizei capacity;
        unsigned char *data;
    protected:
        const GLenum bufferPurpose;
        const GLenum bufferKind;
        const GLsizei itemSize;
    protected:
        virtual void doExpand(const GLsizei oldCapacity, const GLsizei newCapacity);
        void expand();
        void flushRange(const GLsizei minItem, const GLsizei count);
        virtual void freeBuffer();
        virtual void initBuffer();
        virtual bool needsFlush() const { return false; };
        void rangeCheck(const GLsizei index);
        virtual void requireBuffer();
    public:
        virtual void bind();
        virtual void flush();
        void readBack();
        virtual void unbind();
    public:
        GLsizei getCapacity() const { return capacity; }
        GLsizei getItemSize() const { return itemSize; }
};


}
}

#endif
