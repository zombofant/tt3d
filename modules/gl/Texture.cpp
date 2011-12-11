/**********************************************************************
File name: Texture.cpp
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
#include "Texture.hpp"

namespace tt3d {
namespace GL {
    
/* free functions */
void glFramebufferTexture2DNoLOD(GLenum target, GLenum attachment, GLenum toAttach, GLuint texture) 
{
    glFramebufferTexture2D(target, attachment, toAttach, texture, 0);
}

/* tt3d::GL::Texture2D */

Texture2D::Texture2D(const GLenum format, const GLsizei width, 
    const GLsizei height):
    Pixelbuffer::Pixelbuffer(format, width, height, GL_TEXTURE_2D)
{
    glGenTextures(1, &glID);
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
    unbind();
}

Texture2D::~Texture2D() 
{
    glDeleteTextures(1, &glID);
    glID = 0;
}

void Texture2D::bind() 
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, glID);
}

void Texture2D::unbind() 
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

}
}
