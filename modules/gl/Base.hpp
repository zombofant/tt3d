#ifndef _TT3D_GL_BASE_H
#define _TT3D_GL_BASE_H

#include <exception>
#include <GL/glew.h>
#include <string>

namespace tt3d {
namespace GL {
    
class Error: public std::exception {
    public:
        Error(const std::string aMessage);
        ~Error() throw() {};
    private:
        const std::string message;
    public:
        const char *what() {
            return message.c_str();
        }
};

class Struct {
    public:
        virtual void bind() = 0;
        virtual void unbind() = 0;
};

class Class: public Struct {
    public:
        Class();
    protected:
        GLuint glID;
};

void raiseLastGLError();

}
}

#endif
