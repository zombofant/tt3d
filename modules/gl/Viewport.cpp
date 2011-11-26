#include "Viewport.hpp"
#include <GL/glew.h>

namespace tt3d {
namespace GL {

/* tt3d::GL::Viewport */

void Viewport::bind() {
    glViewport(left, top, width, height);
}

void Viewport::unbind() {
    
}

}
}
