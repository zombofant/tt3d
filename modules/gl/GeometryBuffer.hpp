#ifndef _TT3D_GL_GEOMETRY_BUFFERS_H
#define _TT3D_GL_GEOMETRY_BUFFERS_H

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
#include "GenericBuffer.hpp"
#include <string.h>
#include "modules/geometry/Vectors.hpp"

namespace tt3d {
namespace GL {
    
using namespace tt3d;
using namespace tt3d::Geometry;
    
typedef GLsizei VertexIndex;
typedef std::vector<VertexIndex> VertexIndexList;
typedef boost::shared_ptr<VertexIndexList> VertexIndexListHandle;
typedef std::vector<VertexIndexListHandle> VertexIndexListHandleList;

template <class T>
class GenericGeometryBuffer: public GenericBuffer {
    public:
        GenericGeometryBuffer(const GLsizei vertexSize, const GLenum aPurpose):
            GenericBuffer(vertexSize, GL_ARRAY_BUFFER, aPurpose)
        {
            
        };
    private:
        // This is not really cool, but it does the job for now
        BOOST_STATIC_ASSERT_MSG(boost::is_float<T>::value && (sizeof(T) <= 8), "GeometryBuffer template needs a valid opengl float type (i.e. either float or double).");
    protected:        
        static const GLenum glType = ((sizeof(T) == 4)?GL_FLOAT:GL_DOUBLE);
    protected:
        VertexIndexListHandleList handles;
        std::list<VertexIndex> freeVertices;
        std::set<VertexIndex> dirtyVertices;
        
        Utils::BufferMapHandle bufferMap;
    protected:
        virtual void doExpand(const GLsizei oldCapacity, const GLsizei newCapacity) {
            GenericBuffer::doExpand(oldCapacity, newCapacity);
            for (VertexIndex i = oldCapacity; i < newCapacity; i++) {
                freeVertices.push_back(i);
            }
        }
    
        void gc_one(const VertexIndexListHandle handle) {
            VertexIndexList *list = handle.get();
            // freeVertices.splice(freeVertices.begin(), *list);
            for (VertexIndexList::iterator it = list->begin();
                it != list->end();
                it++)
            {
                freeVertices.push_back(*it);
            }
        }

        void get(const GLsizei index, const GLsizei offset, T *value, const GLsizei n) {
            memcpy(value, data + (index * itemSize) + offset * sizeof(T), n * sizeof(T));
            
        }

        GLsizei map(const GLsizei index) {
            if (bufferMap.get()) {
                return bufferMap->map(index);
            } else {
                return index;
            }
        }
        
        void set(const GLsizei index, const GLsizei offset, const T *value, const GLsizei n) {
            memcpy(data + (index * itemSize) + offset * sizeof(T), value, n * sizeof(T));
            dirtyVertices.insert(index);
        }
    public:
        VertexIndexListHandle allocateVertices(const GLsizei count) {
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
            // result->splice(result->begin(), freeVertices, freeVertices.begin(), until);
            
            VertexIndexListHandle handle = VertexIndexListHandle(result);
            handles.push_back(handle);
            return handle;
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

        
        void setMap(boost::shared_ptr<Utils::BufferMap> aValue) {
            bufferMap = aValue;
        }
};

typedef boost::shared_ptr<GenericGeometryBuffer<float> > GenericGeometryBufferHandle;


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
class GeometryBuffer: public GenericGeometryBuffer<T> {
    public:
        GeometryBuffer(const GLenum aPurpose = GL_DYNAMIC_DRAW):
            GenericGeometryBuffer<T>(vertexSize, aPurpose)
        {
            
        };
    private:
        BOOST_STATIC_ASSERT((nPos >= 2) || (nPos == 0));
        BOOST_STATIC_ASSERT(nPos <= 4);
        BOOST_STATIC_ASSERT((nColour == 0) || (nColour >= 3));
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
        static const GLsizei posOffset = 0;
        static const GLsizei colourOffset = nPos;
        static const GLsizei texCoord0Offset = colourOffset + (nColour);
        static const GLsizei texCoord1Offset = texCoord0Offset + (nTexCoord0);
        static const GLsizei texCoord2Offset = texCoord1Offset + (nTexCoord1);
        static const GLsizei texCoord3Offset = texCoord2Offset + (nTexCoord2);
        static const GLsizei normalOffset = texCoord3Offset + (nTexCoord3);
        static const GLsizei vertexAttrib0Offset = normalOffset + (normal?3:0);
        static const GLsizei vertexAttrib1Offset = vertexAttrib0Offset + (nVertexAttrib0);
        static const GLsizei vertexAttrib2Offset = vertexAttrib1Offset + (nVertexAttrib1);
        static const GLsizei vertexAttrib3Offset = vertexAttrib2Offset + (nVertexAttrib2);
        static const GLsizei vertexSize = (vertexAttrib3Offset + (nVertexAttrib3)) * sizeof(T);
        
    public:
        virtual void bind() {
            GenericBuffer::bind();
            if (nPos > 0) {
                glEnableClientState(GL_VERTEX_ARRAY);
                glVertexPointer(nPos, GenericGeometryBuffer<T>::glType, vertexSize, (const void*)(posOffset * sizeof(T)));
            }
            if (nColour > 0) {
                glEnableClientState(GL_COLOR_ARRAY);
                glColorPointer(nColour, GenericGeometryBuffer<T>::glType, vertexSize, (const void*)(colourOffset * sizeof(T)));
                std::cout << nColour << " " << colourOffset * sizeof(T) << " " << vertexSize << std::endl;
            }
            if (nTexCoord0 > 0) {
                glClientActiveTexture(GL_TEXTURE0);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(nTexCoord0, GenericGeometryBuffer<T>::glType, vertexSize, (const void*)texCoord0Offset);
            }
            if (nTexCoord1 > 0) {
                glClientActiveTexture(GL_TEXTURE1);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(nTexCoord1, GenericGeometryBuffer<T>::glType, vertexSize, (const void*)texCoord1Offset);
            }
            if (nTexCoord2 > 0) {
                glClientActiveTexture(GL_TEXTURE2);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(nTexCoord2, GenericGeometryBuffer<T>::glType, vertexSize, (const void*)texCoord2Offset);
            }
            if (nTexCoord3 > 0) {
                glClientActiveTexture(GL_TEXTURE3);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(nTexCoord3, GenericGeometryBuffer<T>::glType, vertexSize, (const void*)texCoord3Offset);
            }
            if (normal) {
                glEnableClientState(GL_NORMAL_ARRAY);
                glTexCoordPointer(3, GenericGeometryBuffer<T>::glType, vertexSize, (const void*)normalOffset);
            }
            if (nVertexAttrib0 > 0) {
                glVertexAttribPointer(0, nVertexAttrib0, GenericGeometryBuffer<T>::glType, GL_FALSE, vertexSize, (const void*)vertexAttrib0Offset);
            }
            if (nVertexAttrib1 > 0) {
                glVertexAttribPointer(1, nVertexAttrib1, GenericGeometryBuffer<T>::glType, GL_FALSE, vertexSize, (const void*)vertexAttrib1Offset);
            }
            if (nVertexAttrib2 > 0) {
                glVertexAttribPointer(2, nVertexAttrib2, GenericGeometryBuffer<T>::glType, GL_FALSE, vertexSize, (const void*)vertexAttrib2Offset);
            }
            if (nVertexAttrib3 > 0) {
                glVertexAttribPointer(3, nVertexAttrib3, GenericGeometryBuffer<T>::glType, GL_FALSE, vertexSize, (const void*)vertexAttrib3Offset);
            }
        }
    
        void getPosition(const GLsizei index, T value[nPos]) {
            BOOST_STATIC_ASSERT(nPos > 0);
            get(GenericGeometryBuffer<T>::map(index), posOffset, value, nPos);
        }
        
        void getPosition(const GLsizei index, Vector2 &value) {
            BOOST_STATIC_ASSERT(nPos == 2);
            BOOST_STATIC_ASSERT(sizeof(T) == 4);
            getPosition(index, value.toVector2f().as_array);
        }
        
        void getPosition(const GLsizei index, Vector3 &value) {
            BOOST_STATIC_ASSERT(nPos == 3);
            BOOST_STATIC_ASSERT(sizeof(T) == 4);
            getPosition(index, value.toVector3f().as_array);
        }
        
        void getColour(const GLsizei index, T value[nColour]) {
            BOOST_STATIC_ASSERT(nColour > 0);
            get(GenericGeometryBuffer<T>::map(index), colourOffset, value, nColour);
        }
        
        void getColour(const GLsizei index, Vector2 &value) {
            BOOST_STATIC_ASSERT(nColour == 2);
            BOOST_STATIC_ASSERT(sizeof(T) == 4);
            getColour(index, value.toVector2f().as_array);
        }
        
        void getColour(const GLsizei index, Vector3 &value) {
            BOOST_STATIC_ASSERT(nColour == 3);
            BOOST_STATIC_ASSERT(sizeof(T) == 4);
            getColour(index, value.toVector3f().as_array);
        }
        
        void getColour(const GLsizei index, Vector4 &value) {
            BOOST_STATIC_ASSERT(nColour == 4);
            BOOST_STATIC_ASSERT(sizeof(T) == 4);
            getColour(index, value.toVector4f().as_array);
        }
        
        void getTexCoord0(const GLsizei index, T value[nTexCoord0]) {
            BOOST_STATIC_ASSERT(nTexCoord0 > 0);
            get(GenericGeometryBuffer<T>::map(index), texCoord0Offset, value, nTexCoord0);
        }
        
        void getTexCoord1(const GLsizei index, T value[nTexCoord1]) {
            BOOST_STATIC_ASSERT(nTexCoord1 > 0);
            get(GenericGeometryBuffer<T>::map(index), texCoord1Offset, value, nTexCoord1);
        }
        
        void getTexCoord2(const GLsizei index, T value[nTexCoord2]) {
            BOOST_STATIC_ASSERT(nTexCoord2 > 0);
            get(GenericGeometryBuffer<T>::map(index), texCoord2Offset, value, nTexCoord2);
        }
        
        void getTexCoord3(const GLsizei index, T value[nTexCoord3]) {
            BOOST_STATIC_ASSERT(nTexCoord3 > 0);
            get(GenericGeometryBuffer<T>::map(index), texCoord3Offset, value, nTexCoord3);
        }
        
        void getNormal(const GLsizei index, T value[3]) {
            BOOST_STATIC_ASSERT(normal);
            get(GenericGeometryBuffer<T>::map(index), normalOffset, value, 3);
        }
        
        void getVertexAttrib0(const GLsizei index, T value[nVertexAttrib0]) {
            BOOST_STATIC_ASSERT(nVertexAttrib0 > 0);
            get(GenericGeometryBuffer<T>::map(index), vertexAttrib0Offset, value, nVertexAttrib0);
        }
        
        void getVertexAttrib1(const GLsizei index, T value[nVertexAttrib1]) {
            BOOST_STATIC_ASSERT(nVertexAttrib1 > 0);
            get(GenericGeometryBuffer<T>::map(index), vertexAttrib1Offset, value, nVertexAttrib1);
        }
        
        void getVertexAttrib2(const GLsizei index, T value[nVertexAttrib2]) {
            BOOST_STATIC_ASSERT(nVertexAttrib2 > 0);
            get(GenericGeometryBuffer<T>::map(index), vertexAttrib2Offset, value, nVertexAttrib2);
        }
        
        void getVertexAttrib3(const GLsizei index, T value[nVertexAttrib3]) {
            BOOST_STATIC_ASSERT(nVertexAttrib3 > 0);
            get(GenericGeometryBuffer<T>::map(index), vertexAttrib3Offset, value, nVertexAttrib3);
        }
        
        
        void setPosition(const GLsizei index, const T value[nPos]) {
            BOOST_STATIC_ASSERT(nPos > 0);
            set(GenericGeometryBuffer<T>::map(index), posOffset, value, nPos);
        }
        
        void setPosition(const GLsizei index, const Vector2 value) {
            BOOST_STATIC_ASSERT(nPos == 2);
            BOOST_STATIC_ASSERT(sizeof(T) == 4);
            setPosition(index, value.toVector2f().as_array);
        }
        
        void setPosition(const GLsizei index, const Vector3 value) {
            BOOST_STATIC_ASSERT(nPos == 3);
            BOOST_STATIC_ASSERT(sizeof(T) == 4);
            setPosition(index, value.toVector3f().as_array);
        }
        
        void setColour(const GLsizei index, const T value[nColour]) {
            BOOST_STATIC_ASSERT(nColour > 0);
            set(GenericGeometryBuffer<T>::map(index), colourOffset, value, nColour);
        }
        
        void setColour(const GLsizei index, const Vector2 value) {
            BOOST_STATIC_ASSERT(nColour == 2);
            BOOST_STATIC_ASSERT(sizeof(T) == 4);
            setColour(index, value.toVector2f().as_array);
        }
        
        void setColour(const GLsizei index, const Vector3 value) {
            BOOST_STATIC_ASSERT(nColour == 3);
            BOOST_STATIC_ASSERT(sizeof(T) == 4);
            setColour(index, value.toVector3f().as_array);
        }
        
        void setColour(const GLsizei index, const Vector4 value) {
            BOOST_STATIC_ASSERT(nColour == 4);
            BOOST_STATIC_ASSERT(sizeof(T) == 4);
            setColour(index, value.toVector4f().as_array);
        }
        
        void setTexCoord0(const GLsizei index, const T value[nTexCoord0]) {
            BOOST_STATIC_ASSERT(nTexCoord0 > 0);
            set(GenericGeometryBuffer<T>::map(index), texCoord0Offset, value, nTexCoord0);
        }
        
        void setTexCoord1(const GLsizei index, const T value[nTexCoord1]) {
            BOOST_STATIC_ASSERT(nTexCoord1 > 0);
            set(GenericGeometryBuffer<T>::map(index), texCoord1Offset, value, nTexCoord1);
        }
        
        void setTexCoord2(const GLsizei index, const T value[nTexCoord2]) {
            BOOST_STATIC_ASSERT(nTexCoord2 > 0);
            set(GenericGeometryBuffer<T>::map(index), texCoord2Offset, value, nTexCoord2);
        }
        
        void setTexCoord3(const GLsizei index, const T value[nTexCoord3]) {
            BOOST_STATIC_ASSERT(nTexCoord3 > 0);
            set(GenericGeometryBuffer<T>::map(index), texCoord3Offset, value, nTexCoord3);
        }
        
        void setNormal(const GLsizei index, const T value[3]) {
            BOOST_STATIC_ASSERT(normal);
            set(GenericGeometryBuffer<T>::map(index), normalOffset, value, 3);
        }
        
        void setVertexAttrib0(const GLsizei index, const T value[nVertexAttrib0]) {
            BOOST_STATIC_ASSERT(nVertexAttrib0 > 0);
            set(GenericGeometryBuffer<T>::map(index), vertexAttrib0Offset, value, nVertexAttrib0);
        }
        
        void setVertexAttrib1(const GLsizei index, const T value[nVertexAttrib1]) {
            BOOST_STATIC_ASSERT(nVertexAttrib1 > 0);
            set(GenericGeometryBuffer<T>::map(index), vertexAttrib1Offset, value, nVertexAttrib1);
        }
        
        void setVertexAttrib2(const GLsizei index, const T value[nVertexAttrib2]) {
            BOOST_STATIC_ASSERT(nVertexAttrib2 > 0);
            set(GenericGeometryBuffer<T>::map(index), vertexAttrib2Offset, value, nVertexAttrib2);
        }
        
        void setVertexAttrib3(const GLsizei index, const T value[nVertexAttrib3]) {
            BOOST_STATIC_ASSERT(nVertexAttrib3 > 0);
            set(GenericGeometryBuffer<T>::map(index), vertexAttrib3Offset, value, nVertexAttrib3);
        }
        
        
        virtual void unbind() {
            if (nPos > 0) {
                glDisableClientState(GL_VERTEX_ARRAY);
            }
            if (nColour > 0) {
                glDisableClientState(GL_COLOR_ARRAY);
            }
            if (nTexCoord0 > 0) {
                glClientActiveTexture(GL_TEXTURE0);
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            }
            if (nTexCoord1 > 0) {
                glClientActiveTexture(GL_TEXTURE1);
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            }
            if (nTexCoord2 > 0) {
                glClientActiveTexture(GL_TEXTURE2);
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            }
            if (nTexCoord3 > 0) {
                glClientActiveTexture(GL_TEXTURE3);
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            }
            if (normal) {
                glDisableClientState(GL_NORMAL_ARRAY);
            }
            if (nVertexAttrib0 > 0) {
                glVertexAttribPointer(0, 0, GenericGeometryBuffer<T>::glType, GL_FALSE, 0, 0);
            }
            if (nVertexAttrib1 > 0) {
                glVertexAttribPointer(1, 0, GenericGeometryBuffer<T>::glType, GL_FALSE, 0, 0);
            }
            if (nVertexAttrib2 > 0) {
                glVertexAttribPointer(2, 0, GenericGeometryBuffer<T>::glType, GL_FALSE, 0, 0);
            }
            if (nVertexAttrib3 > 0) {
                glVertexAttribPointer(3, 0, GenericGeometryBuffer<T>::glType, GL_FALSE, 0, 0);
            }
            GenericBuffer::unbind();
        }
};

}
}

#endif
