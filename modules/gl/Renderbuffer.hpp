#ifndef _TT3D_GL_RENDERBUFFER_H
#define _TT3D_GL_RENDERBUFFER_H

#include "Pixelbuffer.hpp"

namespace tt3d {
namespace GL {

class Renderbuffer: public Pixelbuffer {
    public:
        Renderbuffer(const GLenum format, const GLsizei width, const GLsizei height);
        virtual ~Renderbuffer();
    public:
        virtual glAttachFunc getAttachFunc() { return glFramebufferRenderbuffer; };
        virtual void bind();
        virtual void unbind();
};

}
}

#endif 
