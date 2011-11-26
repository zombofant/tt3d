#include <cstddef>
#include "Base.hpp"

namespace tt3d {
namespace GL {

class Viewport: public Struct {
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
};

}
}
