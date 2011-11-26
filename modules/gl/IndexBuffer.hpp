#ifndef _TT3D_GL_INDEX_BUFFERS_H
#define _TT3D_GL_INDEX_BUFFERS_H

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
#include "GeometryBuffer.hpp"

namespace tt3d {
namespace GL {
    
using namespace tt3d;

struct IndexEntry {
    VertexIndex start;
    const VertexIndexListHandle vertices;
    const VertexIndex count;
    
    IndexEntry(const VertexIndex aStart, const VertexIndexListHandle aVertices);
};
typedef boost::shared_ptr<IndexEntry> IndexEntryHandle;
typedef std::list<IndexEntryHandle> IndexEntryHandles;

class GenericIndexBuffer: public GenericBuffer {
    public:
        GenericIndexBuffer(const GLenum aPurpose);
    protected:
        GLsizei count;
    public:
        virtual void clear();
        void draw(const GLenum mode);
        void drawUnbound(const GLenum mode);
        void dump();
};

class StreamIndexBuffer: public GenericIndexBuffer {
    public:
        StreamIndexBuffer(const GLenum aPurpose = GL_STREAM_DRAW);
    public:
        void add(const VertexIndexListHandle vertices);
};

class StaticIndexBuffer: public GenericIndexBuffer {
    public:
        StaticIndexBuffer(const GLenum aPurpose = GL_DYNAMIC_DRAW);
    private:
        IndexEntryHandles *handles;
    protected:
        void compress();
        void updateHandles(const IndexEntryHandles::iterator startAt, const VertexIndex offset);
    public:
        const IndexEntryHandle add(const VertexIndexListHandle vertices);
        virtual void clear();
        void gc();
        void remove(const IndexEntryHandle handle, const bool autoCompress = true);
};


}
}
#endif
