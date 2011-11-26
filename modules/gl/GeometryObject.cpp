#include "GeometryObject.hpp"

namespace tt3d {
namespace GL {

/* tt3d::GL::GeometryObject */

GeometryObject::GeometryObject(const GLsizei vertexCount, const MaterialHandle material):
    _material(material),
    _vertexCount(vertexCount)
{
    _vertices = _material->getGeometryBuffer()->allocateVertices(vertexCount);
    _staticHandle = _material->getStaticIndexBuffer()->add(_vertices);
}

void GeometryObject::addToStreamBuffer() {
    addToStreamBuffer(_material->getStreamIndexBuffer());
}

void GeometryObject::addToStreamBuffer(StreamIndexBufferHandle indexBuffer) {
    indexBuffer->add(_vertices);
}

void GeometryObject::drawDirect(const GLenum mode) {
    _material->getGeometryBuffer()->bind();
    _material->getStaticIndexBuffer()->drawHandle(_staticHandle, mode);
    _material->getGeometryBuffer()->unbind();
}

}
}
