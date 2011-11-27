#include "GeometryObject.hpp"

namespace tt3d {
namespace GL {
    
using namespace tt3d;

/* tt3d::GL::VertexIndexListMap */

VertexIndexListMap::VertexIndexListMap(const VertexIndexListHandle vertices):
    Utils::BufferMap(),
    _vertices(vertices)
{
    
}

void VertexIndexListMap::rangeCheck(const size_t index) {
    if ((index < 0) || (index >= _vertices->size())) {
        throw 0;
    }
}

size_t VertexIndexListMap::map(const size_t index) {
    const size_t mappedIndex = Utils::BufferMap::map(index);
    rangeCheck(mappedIndex);
    const VertexIndexList *list = _vertices.get();
    return (*list)[mappedIndex];
}

/* tt3d::GL::GeometryObject */

GeometryObject::GeometryObject(const MaterialHandle material):
    _material(material)
{
    
}

void GeometryObject::requireAllocation() {
    if (!_allocation.get())
        _allocation = allocateVertices();
}

void GeometryObject::addToStreamBuffer() {
    addToStreamBuffer(_material->getStreamIndexBuffer());
}

void GeometryObject::addToStreamBuffer(StreamIndexBufferHandle indexBuffer) {
    requireAllocation();
    indexBuffer->add(_allocation->indicies);
}

void GeometryObject::drawDirect(const GLenum mode) {
    requireAllocation();
    _material->getGeometryBuffer()->bind();
    _material->getStaticIndexBuffer()->drawHandle(_allocation->staticHandle, mode);
    _material->getGeometryBuffer()->unbind();
}

Utils::BufferMapHandle GeometryObject::getMap() {
    requireAllocation();
    return Utils::BufferMapHandle(new VertexIndexListMap(_allocation->indicies));
}

/* tt3d::GL::GeometryRaw */

GeometryRaw::GeometryRaw(const MaterialHandle material, const GLsizei vertexCount):
    GeometryObject(material),
    _vertexCount(vertexCount)
{
    
}

VertexAllocationHandle GeometryRaw::allocateVertices() {
    VertexAllocation *allocation = new VertexAllocation(_material->getGeometryBuffer()->allocateVertices(_vertexCount));
    allocation->indicies = VertexIndexListHandle(new VertexIndexList(*allocation->bufferAllocation.get()));
    allocation->staticHandle = _material->getStaticIndexBuffer()->add(allocation->indicies);
    return VertexAllocationHandle(allocation);
}

/* free functions */
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
    buffer->setMap(map);
}

}
}
