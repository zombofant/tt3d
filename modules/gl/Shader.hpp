/**********************************************************************
File name: Shader.hpp
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
#ifndef _TT3D_GL_SHADER_H
#define _TT3D_GL_SHADER_H

#include "Base.hpp"
#include <GL/glew.h>
#include "modules/io/Stream.hpp"

namespace tt3d {
namespace GL {
    
using namespace IO;

class Shader: public Class {
    public:
        Shader(StreamHandle vertexShader, StreamHandle fragmentShader);
        Shader(const std::string &vertexShader, const std::string &fragmentShader);
        virtual ~Shader() {};
    protected:
        GLuint compileShader(const GLenum kind, const std::string &source);
        void loadFromStream(StreamHandle vertexShader, StreamHandle fragmentShader);
        void loadFromString(const std::string &vertexShader, const std::string &fragmentShader);
    public:
        virtual void bind();
        void clear();
        virtual void unbind();
};

}
}

#endif
