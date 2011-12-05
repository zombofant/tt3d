/**********************************************************************
File name: Base.cpp
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
#include <exception>
#include <GL/glew.h>
#include "Base.hpp"

namespace tt3d {
namespace GL {
    
using namespace tt3d;

/* tt3d::GL::Error */

Error::Error(const std::string aMessage):
    Utils::Exception("OpenGL error: "+aMessage)
{
    
}

/* tt3d::GL::Class */

Class::Class():
    glID(0)
{
    
}

/* free functions */

void raiseLastGLError() {
    const GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        throw Error((const char *)gluErrorString(error));
    }
}

}
}
