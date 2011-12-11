#ifndef _TT3D_GL_TEXTURE_H
#define _TT3D_GL_TEXTURE_H

#include <GL/glew.h>
#include "Pixelbuffer.hpp"

namespace tt3d {
namespace GL {

void glFramebufferTexture2DNoLOD(GLenum target, GLenum attachment, GLenum toAttach, GLuint texture);

class Texture2D: public Pixelbuffer {
    public:
        Texture2D(const GLenum format, const GLsizei width, const GLsizei height);
        virtual ~Texture2D();
    public:
        virtual glAttachFunc getAttachFunc() { return glFramebufferTexture2DNoLOD; };
        virtual void bind();
        virtual void unbind();
};

}
}

#endif
