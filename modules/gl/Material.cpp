/**********************************************************************
File name: Material.cpp
This file is part of: tt3d — Freeform transport simulation

LICENSE

The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations under
the License.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public license (the  "GPL License"), in which case  the
provisions of GPL License are applicable instead of those above.

FEEDBACK & QUESTIONS

For feedback and questions about tt3d please e-mail one of the authors
named in the AUTHORS file.
**********************************************************************/
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
