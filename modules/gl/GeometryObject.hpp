#ifndef _TT3D_GL_GEOMETRY_OBJECT_H
#define _TT3D_GL_GEOMETRY_OBJECT_H

#include <GL/glew.h>
#include "IndexBuffer.hpp"
#include "Material.hpp"
#include "modules/utils/BufferMap.hpp"


namespace tt3d {
namespace GL {
    
struct VertexAllocation {
    const VertexIndexListHandle bufferAllocation;
    VertexIndexListHandle indicies;
    IndexEntryHandle staticHandle;
    
    VertexAllocation(const VertexIndexListHandle aBufferAllocation):
        bufferAllocation(aBufferAllocation) {};
};
    
typedef boost::shared_ptr<VertexAllocation> VertexAllocationHandle;

class VertexIndexListMap: public Utils::BufferMap {
    public:
        VertexIndexListMap(const VertexIndexListHandle vertices);
    private:
        const VertexIndexListHandle _vertices;
    protected:
        void rangeCheck(const size_t index);
    public:
        virtual size_t map(const size_t index);
};

class GeometryObject {
    public:
        GeometryObject(const MaterialHandle material);
    protected:
        const MaterialHandle _material;
        VertexAllocationHandle _allocation;
    protected:
        virtual VertexAllocationHandle allocateVertices() = 0;
        void requireAllocation();
    public:
        void addToStreamBuffer();
        void addToStreamBuffer(StreamIndexBufferHandle indexBuffer);
        void drawDirect(const GLenum mode);
        virtual Utils::BufferMapHandle getMap();
        const MaterialHandle getMaterial() const { return _material; }
};

class GeometryRaw: public GeometryObject {
    public:
        GeometryRaw(const MaterialHandle material, const GLsizei vertexCount);
    protected:
        const GLsizei _vertexCount;
    protected:
        virtual VertexAllocationHandle allocateVertices();
    public:
        GLsizei getVertexCount() { return _vertexCount; }
};

typedef boost::shared_ptr<GeometryObject> GeometryObjectHandle;

template <class T>
void getMappedBuffer(const GeometryObjectHandle handle, 
    GenericGeometryBufferHandle &bufferHandle, 
    T *&buffer, 
    Utils::BufferMapHandle &mapHandle,
    Utils::BufferMap *&map)
{
    bufferHandle = handle->getMaterial()->getGeometryBuffer();
    buffer = dynamic_cast<T*>(bufferHandle.get());
    mapHandle = handle->getMap();
    map = mapHandle.get();
    buffer->setMap(mapHandle);
}

}
}
#endif
