/**********************************************************************
File name: Pixelbuffer.hpp
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
#ifndef _TT3D_GL_PIXELBUFFER_H
#define _TT3D_GL_PIXELBUFFER_H

#include <GL/glew.h>
#include "Base.hpp"
#include <boost/shared_ptr.hpp>

namespace tt3d {
namespace GL {
    
typedef void (GLAPIENTRY * glAttachFunc) (GLenum, GLenum, GLenum, GLuint);

class Pixelbuffer: public Class {
    public:
        Pixelbuffer(const GLenum format, const GLsizei width, 
            const GLsizei height, const GLenum bufferKind);
    private:
        const GLenum _format;
        const GLsizei _width, _height;
        const GLenum _bufferKind;
    public:
        virtual glAttachFunc getAttachFunc() const = 0;
        GLenum getBufferKind() const { return _bufferKind; };
        virtual GLenum getFormat() const { return _format; };
        virtual GLsizei getHeight() const { return _height; };
        virtual GLsizei getWidth() const { return _width; };
};

typedef boost::shared_ptr<Pixelbuffer> PixelbufferHandle;

}
}

#endif
