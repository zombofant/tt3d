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
