#include "Viewport.hpp"
#include <GL/glew.h>

namespace tt3d {
namespace GL {

/* tt3d::GL::Viewport */

Viewport::Viewport(const GLint aLeft, const GLint aTop,
    const GLsizei aWidth, const GLsizei aHeight):
        left(aLeft),
        top(aTop),
        width(aWidth),
        height(aHeight)
{
}

void Viewport::bind() {
    glViewport(left, top, width, height);
}

void Viewport::unbind() {
    
}

}
}
