#include "Viewport.hpp"
#include <GL/glew.h>

namespace tt3d {
namespace GL {
    
using namespace tt3d;

/* tt3d::GL::Viewport */

Viewport::Viewport():
    left(0),
    top(0),
    width(0),
    height(0)
{
    
}

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

void Viewport::setRect(const Geometry::Rect &aValue) {
    left = aValue.x;
    top = aValue.y;
    width = aValue.w;
    height = aValue.h;
}

}
}
