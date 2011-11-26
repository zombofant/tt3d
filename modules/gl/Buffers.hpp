#ifndef _TT3D_GL_BUFFERS_H
#define _TT3D_GL_BUFFERS_H

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
    
typedef GLsizei VertexIndex;
typedef std::list<VertexIndex> VertexIndexList;
typedef boost::shared_ptr<VertexIndexList> VertexIndexListHandle;
typedef std::vector<VertexIndexListHandle> VertexIndexListHandleList;

struct IndexEntry {
    VertexIndex start;
    const VertexIndexListHandle vertices;
    const VertexIndex count;
    
    IndexEntry(const VertexIndex aStart, const VertexIndexListHandle aVertices);
};
typedef boost::shared_ptr<IndexEntry> IndexEntryHandle;
typedef std::list<IndexEntryHandle> IndexEntryHandles;

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
        void expand();
        void flushRange(const GLsizei minItem, const GLsizei count);
        virtual void freeBuffer() {};
        virtual void initBuffer() {};
        virtual bool needsFlush() const { return false; };
        void rangeCheck(const GLsizei index);
    public:
        virtual void bind();
        virtual void flush();
        void readBack();
        virtual void unbind();
    public:
        GLsizei getCapacity() const { return capacity; }
        GLsizei getItemSize() const { return itemSize; }
};

template <
    class T, 
    unsigned int nPos = 0, 
    unsigned int nColour = 0, 
    unsigned int nTexCoord0 = 0, 
    unsigned int nTexCoord1 = 0, 
    unsigned int nTexCoord2 = 0, 
    unsigned int nTexCoord3 = 0, 
    bool normal = false, 
    unsigned int nVertexAttrib0 = 0, 
    unsigned int nVertexAttrib1 = 0, 
    unsigned int nVertexAttrib2 = 0, 
    unsigned int nVertexAttrib3 = 0>
class GeometryBuffer: public GenericBuffer {
    public:
        GeometryBuffer(const GLenum aPurpose = GL_DYNAMIC_DRAW):
            GenericBuffer(vertexSize, GL_ARRAY_BUFFER, aPurpose)
        {
            
        };
    private:
        // This is not really cool, but it does the job for now
        BOOST_STATIC_ASSERT_MSG(boost::is_float<T>::value && (sizeof(T) <= 8), "GeometryBuffer template needs a valid opengl float type (i.e. either float or double).");
        BOOST_STATIC_ASSERT(nPos <= 4);
        BOOST_STATIC_ASSERT(nColour <= 4);
        BOOST_STATIC_ASSERT(nTexCoord0 <= 4);
        BOOST_STATIC_ASSERT(nTexCoord1 <= 4);
        BOOST_STATIC_ASSERT(nTexCoord2 <= 4);
        BOOST_STATIC_ASSERT(nTexCoord3 <= 4);
        BOOST_STATIC_ASSERT(nVertexAttrib0 <= 4);
        BOOST_STATIC_ASSERT(nVertexAttrib1 <= 4);
        BOOST_STATIC_ASSERT(nVertexAttrib2 <= 4);
        BOOST_STATIC_ASSERT(nVertexAttrib3 <= 4);
    private:
        VertexIndexListHandleList handles;
        VertexIndexList freeVertices;
        std::set<VertexIndex> dirtyVertices;
    private:
        static const GLsizei posOffset = 0;
        static const GLsizei colourOffset = nPos;
        static const GLsizei texCoord0Offset = colourOffset + (nColour);
        static const GLsizei texCoord1Offset = texCoord0Offset + (nTexCoord0);
        static const GLsizei texCoord2Offset = texCoord1Offset + (nTexCoord1);
        static const GLsizei texCoord3Offset = texCoord2Offset + (nTexCoord2);
        static const GLsizei normalOffset = texCoord3Offset + (nTexCoord3);
        static const GLsizei vertexAttrib0Offset = normalOffset + 3;
        static const GLsizei vertexAttrib1Offset = vertexAttrib0Offset + (nVertexAttrib0);
        static const GLsizei vertexAttrib2Offset = vertexAttrib1Offset + (nVertexAttrib1);
        static const GLsizei vertexAttrib3Offset = vertexAttrib2Offset + (nVertexAttrib2);
        static const GLsizei vertexSize = (vertexAttrib3Offset + (nVertexAttrib3)) * sizeof(T);
        static const GLenum glType = ((sizeof(T) == 4)?GL_FLOAT:GL_DOUBLE);
        
        boost::shared_ptr<Utils::BufferMap> bufferMap;
    protected:
        virtual void freeBuffer() {
            glDeleteBuffers(1, &glID);
        }
    
        void gc_one(const VertexIndexListHandle handle) {
            VertexIndexList &list = handle.get();
            freeVertices.splice(freeVertices.begin(), list);
        }

        void get(const GLsizei index, T *value, const GLsizei n) {
            memcpy(value, data + (index * itemSize), n * sizeof(T));
            
        }
        
        virtual void initBuffer() {
            glGenBuffers(1, &glID);
            if (capacity > 0) {
                glBindBuffer(GL_ARRAY_BUFFER, glID);
                glBufferData(GL_ARRAY_BUFFER, capacity * vertexSize, 0, bufferPurpose);
            }
        }

        GLsizei map(const GLsizei index) {
            if (bufferMap.get()) {
                return bufferMap->map(index);
            } else {
                return index;
            }
        }
        
        void set(const GLsizei index, const T *value, const GLsizei n) {
            memcpy(data + (index * itemSize), value, n * sizeof(T));
            dirtyVertices.insert(index);
        }
    public:
        VertexIndexList allocateVertices(const GLsizei count) {
            if (freeVertices.size() < count) {
                gc();
                while (freeVertices.size() < count) {
                    expand();
                }
            }
            
            VertexIndexList::iterator until = freeVertices.begin();
            advance(until, count);
            VertexIndexList result;
            result.splice(result.begin(), freeVertices, freeVertices.begin(), until);
            return result;
        }
        
        void gc() {
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
    public:
        virtual void bind() {
            GenericBuffer::bind();
            if (nPos > 0) {
                glEnableClientState(GL_VERTEX_ARRAY);
                glVertexPointer(nPos, glType, vertexSize, (const void*)posOffset);
            }
            if (nColour > 0) {
                glEnableClientState(GL_COLOR_ARRAY);
                glColorPointer(nColour, glType, vertexSize, (const void*)colourOffset);
            }
            if (nTexCoord0 > 0) {
                glClientActiveTexture(GL_TEXTURE0);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(nTexCoord0, glType, vertexSize, (const void*)texCoord0Offset);
            }
            if (nTexCoord1 > 0) {
                glClientActiveTexture(GL_TEXTURE1);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(nTexCoord1, glType, vertexSize, (const void*)texCoord1Offset);
            }
            if (nTexCoord2 > 0) {
                glClientActiveTexture(GL_TEXTURE2);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(nTexCoord2, glType, vertexSize, (const void*)texCoord2Offset);
            }
            if (nTexCoord3 > 0) {
                glClientActiveTexture(GL_TEXTURE3);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(nTexCoord3, glType, vertexSize, (const void*)texCoord3Offset);
            }
            if (normal) {
                glEnableClientState(GL_NORMAL_ARRAY);
                glTexCoordPointer(3, glType, vertexSize, (const void*)normalOffset);
            }
            if (nVertexAttrib0 > 0) {
                glVertexAttribPointer(0, nVertexAttrib0, glType, GL_FALSE, vertexSize, (const void*)vertexAttrib0Offset);
            }
            if (nVertexAttrib1 > 0) {
                glVertexAttribPointer(1, nVertexAttrib1, glType, GL_FALSE, vertexSize, (const void*)vertexAttrib1Offset);
            }
            if (nVertexAttrib2 > 0) {
                glVertexAttribPointer(2, nVertexAttrib2, glType, GL_FALSE, vertexSize, (const void*)vertexAttrib2Offset);
            }
            if (nVertexAttrib3 > 0) {
                glVertexAttribPointer(3, nVertexAttrib3, glType, GL_FALSE, vertexSize, (const void*)vertexAttrib3Offset);
            }
        }
    
        void getPosition(const GLsizei index, T value[nPos]) {
            BOOST_STATIC_ASSERT(nPos > 0);
            get(map(index) + posOffset, value, nPos);
        }
        
        void getColour(const GLsizei index, T value[nColour]) {
            BOOST_STATIC_ASSERT(nColour > 0);
            get(map(index) + colourOffset, value, nPos);
        }
        
        void getTexCoord0(const GLsizei index, T value[nTexCoord0]) {
            BOOST_STATIC_ASSERT(nTexCoord0 > 0);
            get(map(index) + texCoord0Offset, value, nPos);
        }
        
        void getTexCoord1(const GLsizei index, T value[nTexCoord1]) {
            BOOST_STATIC_ASSERT(nTexCoord1 > 0);
            get(map(index) + texCoord1Offset, value, nPos);
        }
        
        void getTexCoord2(const GLsizei index, T value[nTexCoord2]) {
            BOOST_STATIC_ASSERT(nTexCoord2 > 0);
            get(map(index) + texCoord2Offset, value, nPos);
        }
        
        void getTexCoord3(const GLsizei index, T value[nTexCoord3]) {
            BOOST_STATIC_ASSERT(nTexCoord3 > 0);
            get(map(index) + texCoord3Offset, value, nPos);
        }
        
        void getNormal(const GLsizei index, T value[3]) {
            BOOST_STATIC_ASSERT(normal);
            get(map(index) + normalOffset, value, nPos);
        }
        
        void getVertexAttrib0(const GLsizei index, T value[nVertexAttrib0]) {
            BOOST_STATIC_ASSERT(nVertexAttrib0 > 0);
            get(map(index) + vertexAttrib0Offset, value, nPos);
        }
        
        void getVertexAttrib1(const GLsizei index, T value[nVertexAttrib1]) {
            BOOST_STATIC_ASSERT(nVertexAttrib1 > 0);
            get(map(index) + vertexAttrib1Offset, value, nPos);
        }
        
        void getVertexAttrib2(const GLsizei index, T value[nVertexAttrib2]) {
            BOOST_STATIC_ASSERT(nVertexAttrib2 > 0);
            get(map(index) + vertexAttrib2Offset, value, nPos);
        }
        
        void getVertexAttrib3(const GLsizei index, T value[nVertexAttrib3]) {
            BOOST_STATIC_ASSERT(nVertexAttrib3 > 0);
            get(map(index) + vertexAttrib3Offset, value, nPos);
        }
        
        boost::shared_ptr<Utils::BufferMap> getMap() {
            return bufferMap;
        }
        
        
        virtual void flush() {
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
        
        
        void setPosition(const GLsizei index, const T value[nPos]) {
            BOOST_STATIC_ASSERT(nPos > 0);
            set(map(index) + posOffset, value, nPos);
        }
        
        void setColour(const GLsizei index, const T value[nColour]) {
            BOOST_STATIC_ASSERT(nColour > 0);IndexEntryHandle
            set(map(index) + colourOffset, value, nPos);
        }
        
        void setTexCoord0(const GLsizei index, const T value[nTexCoord0]) {
            BOOST_STATIC_ASSERT(nTexCoord0 > 0);
            set(map(index) + texCoord0Offset, value, nPos);
        }
        
        void setTexCoord1(const GLsizei index, const T value[nTexCoord1]) {
            BOOST_STATIC_ASSERT(nTexCoord1 > 0);
            set(map(index) + texCoord1Offset, value, nPos);
        }
        
        void setTexCoord2(const GLsizei index, const T value[nTexCoord2]) {
            BOOST_STATIC_ASSERT(nTexCoord2 > 0);
            set(map(index) + texCoord2Offset, value, nPos);
        }
        
        void setTexCoord3(const GLsizei index, const T value[nTexCoord3]) {
            BOOST_STATIC_ASSERT(nTexCoord3 > 0);
            set(map(index) + texCoord3Offset, value, nPos);
        }
        
        void setNormal(const GLsizei index, const T value[3]) {
            BOOST_STATIC_ASSERT(normal);
            set(map(index) + normalOffset, value, nPos);
        }
        
        void setVertexAttrib0(const GLsizei index, const T value[nVertexAttrib0]) {
            BOOST_STATIC_ASSERT(nVertexAttrib0 > 0);
            set(map(index) + vertexAttrib0Offset, value, nPos);
        }
        
        void setVertexAttrib1(const GLsizei index, const T value[nVertexAttrib1]) {
            BOOST_STATIC_ASSERT(nVertexAttrib1 > 0);
            set(map(index) + vertexAttrib1Offset, value, nPos);
        }
        
        void setVertexAttrib2(const GLsizei index, const T value[nVertexAttrib2]) {
            BOOST_STATIC_ASSERT(nVertexAttrib2 > 0);
            set(map(index) + vertexAttrib2Offset, value, nPos);
        }
        
        void setVertexAttrib3(const GLsizei index, const T value[nVertexAttrib3]) {
            BOOST_STATIC_ASSERT(nVertexAttrib3 > 0);
            set(map(index) + vertexAttrib3Offset, value, nPos);
        }
        
        void setMap(boost::shared_ptr<Utils::BufferMap> aValue) {
            bufferMap = aValue;
        }
};

class GenericIndexBuffer: public GenericBuffer {
    public:
        GenericIndexBuffer(const GLenum aPurpose);
    protected:
        GLsizei count;
    public:
        virtual void clear();
        void draw(const GLenum mode);
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
