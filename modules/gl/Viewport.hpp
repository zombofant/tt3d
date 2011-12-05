/**********************************************************************
File name: Viewport.hpp
This file is part of: tt3d — Freeform transport simulation

LICENSE

The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations under
the License.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public license (the  "GPL License"), in which case  the
provisions of GPL License are applicable instead of those above.

FEEDBACK & QUESTIONS

For feedback and questions about tt3d please e-mail one of the authors
named in the AUTHORS file.
**********************************************************************/
#ifndef _TT3D_GL_VIEWPORT_H
#define _TT3D_GL_VIEWPORT_H

#include <cstddef>
#include "Base.hpp"
#include "modules/math/Rect.hpp"
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
        
        void setRect(const Math::Rect &aRect);
};

typedef boost::shared_ptr<Viewport> ViewportHandle;

}
}

#endif
