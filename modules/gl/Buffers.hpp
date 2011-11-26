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
        static const GLsizei colourOffset = nPos;
        static const GLsizei texCoord0Offset = colourOffset + (nColour);
        static const GLsizei texCoord1Offset = texCoord0Offset + (nTexCoord0);
        static const GLsizei texCoord2Offset = texCoord1Offset + (nTexCoord1);
        static const GLsizei texCoord3Offset = texCoord2Offset + (nTexCoord2);
        static const GLsizei normalOffset = texCoord3Offset + (nTexCoord3);
        static const GLsizei vertexParam0Offset = normalOffset + 3;
        static const GLsizei vertexParam1Offset = vertexParam0Offset + (nVertexParam0);
        static const GLsizei vertexParam2Offset = vertexParam1Offset + (nVertexParam1);
        static const GLsizei vertexParam3Offset = vertexParam2Offset + (nVertexParam2);
        static const GLsizei vertexSize = vertexParam3Offset + (nVertexParam3);
        
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
        
        void getVertexParam0(const GLsizei index, T value[nVertexParam0]) {
            BOOST_STATIC_ASSERT(nVertexParam0 > 0);
            get(map(index) + vertexParam0Offset, value, nPos);
        }
        
        void getVertexParam1(const GLsizei index, T value[nVertexParam1]) {
            BOOST_STATIC_ASSERT(nVertexParam1 > 0);
            get(map(index) + vertexParam1Offset, value, nPos);
        }
        
        void getVertexParam2(const GLsizei index, T value[nVertexParam2]) {
            BOOST_STATIC_ASSERT(nVertexParam2 > 0);
            get(map(index) + vertexParam2Offset, value, nPos);
        }
        
        void getVertexParam3(const GLsizei index, T value[nVertexParam3]) {
            BOOST_STATIC_ASSERT(nVertexParam3 > 0);
            get(map(index) + vertexParam3Offset, value, nPos);
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
        }
        
        
        void setPosition(const GLsizei index, const T value[nPos]) {
            BOOST_STATIC_ASSERT(nPos > 0);
            set(map(index) + posOffset, value, nPos);
        }
        
        void setColour(const GLsizei index, const T value[nColour]) {
            BOOST_STATIC_ASSERT(nColour > 0);
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
        
        void setVertexParam0(const GLsizei index, const T value[nVertexParam0]) {
            BOOST_STATIC_ASSERT(nVertexParam0 > 0);
            set(map(index) + vertexParam0Offset, value, nPos);
        }
        
        void setVertexParam1(const GLsizei index, const T value[nVertexParam1]) {
            BOOST_STATIC_ASSERT(nVertexParam1 > 0);
            set(map(index) + vertexParam1Offset, value, nPos);
        }
        
        void setVertexParam2(const GLsizei index, const T value[nVertexParam2]) {
            BOOST_STATIC_ASSERT(nVertexParam2 > 0);
            set(map(index) + vertexParam2Offset, value, nPos);
        }
        
        void setVertexParam3(const GLsizei index, const T value[nVertexParam3]) {
            BOOST_STATIC_ASSERT(nVertexParam3 > 0);
            set(map(index) + vertexParam3Offset, value, nPos);
        }
        
        void setMap(boost::shared_ptr<Utils::BufferMap> aValue) {
            bufferMap = aValue;
        }
};

}
}

#endif
