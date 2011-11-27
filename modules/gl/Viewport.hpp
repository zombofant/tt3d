#ifndef _TT3D_GL_VIEWPORT_H
#define _TT3D_GL_VIEWPORT_H

#include <cstddef>
#include "Base.hpp"
#include "modules/geometry/Rect.hpp"
#include <boost/shared_ptr.hpp>

namespace tt3d {
namespace GL {
    
using namespace tt3d;

class Viewport: public Struct {
    public:
        Viewport();
        Viewport(const GLint aLeft, 
            const GLint aTop, 
            const GLsizei aWidth, 
            const GLsizei aHeight);
    private:
        GLint left, top;
        GLsizei width, height;
    public:
        void bind();
        void unbind();
    public:
        GLint getLeft() const { return left; };
        GLsizei getHeight() const { return height; }
        GLint getTop() const { return top; };
        GLsizei getWidth() const { return width; }
        
        void setLeft(const GLint aValue) { left = aValue; };
        void setHeight(const GLsizei aValue) { height = aValue; };
        void setTop(const GLint aValue) { top = aValue; };
        void setWidth(const GLsizei aValue) { width = aValue; };
        
        void setRect(const Geometry::Rect &aRect);
};

typedef boost::shared_ptr<Viewport> ViewportHandle;

}
}

#endif
