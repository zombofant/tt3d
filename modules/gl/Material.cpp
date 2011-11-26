#include "Material.hpp"

namespace tt3d {
namespace GL {

/* tt3d::GL::Material */

Material::Material(GenericGeometryBufferHandle geometryBuffer):
    _geometryBuffer(geometryBuffer)
{
    
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
