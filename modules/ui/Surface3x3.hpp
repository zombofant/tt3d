/**********************************************************************
File name: Surface3x3.hpp
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
#ifndef _TT3D_UI_SURFACE3X3_H
#define _TT3D_UI_SURFACE3X3_H

#include "modules/math/Vectors.hpp"
#include "modules/math/Rect.hpp"
#include "Surface.hpp"

namespace tt3d {
namespace UI {
    
using namespace tt3d;
using namespace Math;

class Surface3x3: public Surface {
    public:
        Surface3x3(const MaterialHandle material);
    public:
        struct QuadInfo {
            Vector4 colours[4];
            Vector2 texCoords[4];
            
            QuadInfo();
            
            void apply(GeometryBufferDriver *bufferDriver) const;
            void setAllColours(const Vector4 &colour);
            void setAllTexCoords(const Vector2 &texCoord);
        };
    protected:
        QuadInfo _quads[9];
        Borders _margin;
    public:
        QuadInfo &getQuad(const int index) { return _quads[index]; };
        Borders &getBorders() { return _margin; };
        virtual void updateGeometry(const Rect rect, GeometryObjectHandle &geometry);
};

}
}

#endif
