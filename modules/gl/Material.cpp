#include "Material.hpp"

namespace tt3d {
namespace GL {

/* tt3d::GL::Material */

Material::Material(GenericGeometryBufferHandle geometryBuffer):
    _geometryBuffer(geometryBuffer),
    _staticIndexBuffer(new StaticIndexBuffer(GL_DYNAMIC_DRAW)),
    _streamIndexBuffer(new StreamIndexBuffer())
{
    
}

void Material::bind() {
    bind(false);
}


void Material::bind(const bool useStream) {
    _boundAsStream = useStream;
    _geometryBuffer->bind();
    if (_boundAsStream) {
        _streamIndexBuffer->bind();
    } else {
        _staticIndexBuffer->bind();
    }
}

void Material::render(const GLenum mode) {
    /*  if FBoundAsStream then
  begin
    glDrawElements(AsType, FStreamIndexBuffer.Count, GL_UNSIGNED_INT, nil);
    FStreamIndexBuffer.Clear;
  end
  else
    glDrawElements(AsType, FStaticIndexBuffer.Count, GL_UNSIGNED_INT, nil);
*/
    if (_boundAsStream) {
        glDrawElements(mode, _streamIndexBuffer->getCount(), GL_UNSIGNED_INT, 0);
        _streamIndexBuffer->clear();
    } else {
        glDrawElements(mode, _staticIndexBuffer->getCount(), GL_UNSIGNED_INT, 0);
    }
}

void Material::unbind() {
    if (_boundAsStream) {
        _streamIndexBuffer->unbind();
        _streamIndexBuffer->clear();
    } else {
        _staticIndexBuffer->unbind();
    }
    _geometryBuffer->unbind();
}

}
}
