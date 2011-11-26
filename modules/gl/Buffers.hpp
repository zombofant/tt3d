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
        virtual void freeBuffer() {};
        virtual void initBuffer() {};
        virtual bool needsFlush() const { return false; };
        void rangeCheck(const GLsizei index);
    public:
        void bind();
        virtual void flush();
        void flushRange(const GLsizei minItem, const GLsizei count);
        void readBack();
        void unbind();
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
    unsigned int nVertexParam0 = 0, 
    unsigned int nVertexParam1 = 0, 
    unsigned int nVertexParam2 = 0, 
    unsigned int nVertexParam3 = 0>
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
        BOOST_STATIC_ASSERT(nVertexParam0 <= 4);
        BOOST_STATIC_ASSERT(nVertexParam1 <= 4);
        BOOST_STATIC_ASSERT(nVertexParam2 <= 4);
        BOOST_STATIC_ASSERT(nVertexParam3 <= 4);
    private:
        VertexIndexListHandleList handles;
        VertexIndexList freeVertices;
        std::set<VertexIndex> dirtyVertices;
    private:
        static const GLsizei posOffset = 0;
        static const GLsizei colourOffset = sizeof(T) * nPos;
        static const GLsizei texCoord0Offset = colourOffset + sizeof(T) * (nColour);
        static const GLsizei texCoord1Offset = texCoord0Offset + sizeof(T) * (nTexCoord0);
        static const GLsizei texCoord2Offset = texCoord1Offset + sizeof(T) * (nTexCoord1);
        static const GLsizei texCoord3Offset = texCoord2Offset + sizeof(T) * (nTexCoord2);
        static const GLsizei normalOffset = texCoord3Offset + sizeof(T) * (nTexCoord3);
        static const GLsizei vertexParam0Offset = normalOffset + sizeof(T) * 3;
        static const GLsizei vertexParam1Offset = vertexParam0Offset + sizeof(T) * (nVertexParam0);
        static const GLsizei vertexParam2Offset = vertexParam1Offset + sizeof(T) * (nVertexParam1);
        static const GLsizei vertexParam3Offset = vertexParam2Offset + sizeof(T) * (nVertexParam2);
        static const GLsizei vertexSize = vertexParam3Offset + sizeof(T) * (nVertexParam3);
        
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
        void getPosition(const GLsizei index, T value[nPos]) {
            BOOST_STATIC_ASSERT(nPos > 0);
            get(map(index) + posOffset, value, nPos);
        }
        
        void getColour(const GLsizei index, T value[nColour]) {
            BOOST_STATIC_ASSERT(nColour > 0);
        }
        
        void getTexCoord0(const GLsizei index, T value[nTexCoord0]) {
            BOOST_STATIC_ASSERT(nTexCoord0 > 0);
        }
        
        void getTexCoord1(const GLsizei index, T value[nTexCoord1]) {
            BOOST_STATIC_ASSERT(nTexCoord1 > 0);
        }
        
        void getTexCoord2(const GLsizei index, T value[nTexCoord2]) {
            BOOST_STATIC_ASSERT(nTexCoord2 > 0);
        }
        
        void getTexCoord3(const GLsizei index, T value[nTexCoord3]) {
            BOOST_STATIC_ASSERT(nTexCoord3 > 0);
        }
        
        void getNormal(const GLsizei index, T value[3]) {
            BOOST_STATIC_ASSERT(normal);
        }
        
        void getVertexParam0(const GLsizei index, T value[nVertexParam0]) {
            BOOST_STATIC_ASSERT(nVertexParam0 > 0);
        }
        
        void getVertexParam1(const GLsizei index, T value[nVertexParam1]) {
            BOOST_STATIC_ASSERT(nVertexParam1 > 0);
        }
        
        void getVertexParam2(const GLsizei index, T value[nVertexParam2]) {
            BOOST_STATIC_ASSERT(nVertexParam2 > 0);
        }
        
        void getVertexParam3(const GLsizei index, T value[nVertexParam3]) {
            BOOST_STATIC_ASSERT(nVertexParam3 > 0);
        }
        
        boost::shared_ptr<Utils::BufferMap> getMap() {
            return bufferMap;
        }
        
        
        void setPosition(const GLsizei index, const T value[nPos]) {
            BOOST_STATIC_ASSERT(nPos > 0);
        }
        
        void setColour(const GLsizei index, const T value[nColour]) {
            BOOST_STATIC_ASSERT(nColour > 0);
        }
        
        void setTexCoord0(const GLsizei index, const T value[nTexCoord0]) {
            BOOST_STATIC_ASSERT(nTexCoord0 > 0);
        }
        
        void setTexCoord1(const GLsizei index, const T value[nTexCoord1]) {
            BOOST_STATIC_ASSERT(nTexCoord1 > 0);
        }
        
        void setTexCoord2(const GLsizei index, const T value[nTexCoord2]) {
            BOOST_STATIC_ASSERT(nTexCoord2 > 0);
        }
        
        void setTexCoord3(const GLsizei index, const T value[nTexCoord3]) {
            BOOST_STATIC_ASSERT(nTexCoord3 > 0);
        }
        
        void setNormal(const GLsizei index, const T value[3]) {
            BOOST_STATIC_ASSERT(normal);
        }
        
        void setVertexParam0(const GLsizei index, const T value[nVertexParam0]) {
            BOOST_STATIC_ASSERT(nVertexParam0 > 0);
        }
        
        void setVertexParam1(const GLsizei index, const T value[nVertexParam1]) {
            BOOST_STATIC_ASSERT(nVertexParam1 > 0);
        }
        
        void setVertexParam2(const GLsizei index, const T value[nVertexParam2]) {
            BOOST_STATIC_ASSERT(nVertexParam2 > 0);
        }
        
        void setVertexParam3(const GLsizei index, const T value[nVertexParam3]) {
            BOOST_STATIC_ASSERT(nVertexParam3 > 0);
        }
        
        void setMap(boost::shared_ptr<Utils::BufferMap> aValue) {
            bufferMap = aValue;
        }
};

}
}

#endif
