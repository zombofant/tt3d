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
