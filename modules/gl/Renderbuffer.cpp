#include "Renderbuffer.hpp"

namespace tt3d {
namespace GL {
    
/* tt3d::GL::Renderbuffer */

Renderbuffer::Renderbuffer(const GLenum format, const GLsizei width, 
    const GLsizei height):
    Pixelbuffer::Pixelbuffer(format, width, height, GL_RENDERBUFFER)
{
    glGenRenderbuffers(1, &glID);
    bind();
    glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
    unbind();
}

Renderbuffer::~Renderbuffer() 
{
    glDeleteRenderbuffers(1, &glID);
}

void Renderbuffer::bind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, glID);
}

void Renderbuffer::unbind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

}
}
