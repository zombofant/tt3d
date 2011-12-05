/**********************************************************************
File name: Surface.hpp
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
#ifndef _TT3D_UI_SURFACE_H
#define _TT3D_UI_SURFACE_H

#include "modules/math/Rect.hpp"
#include "modules/gl/GeometryObject.hpp"
#include "modules/gl/Material.hpp"

namespace tt3d {
namespace UI {

using namespace tt3d;
using namespace tt3d::GL;
using namespace tt3d::Math;

/*class GeometryBuffer: public GL::GeometryBuffer<float, 2, 4, 2> {
    
};*/
typedef GL::GeometryBuffer<float, 2, 4, 2> GeometryBuffer;

class Material: public GL::Material {
    public:
        Material();
};
typedef boost::shared_ptr<Material> MaterialHandle;

class Surface {
    public:
        Surface(const MaterialHandle material);
    protected:
        const MaterialHandle _material;
    public:
        const MaterialHandle getMaterial() const { return _material; };
        virtual void updateGeometry(const Rect rect, GeometryObjectHandle &geometry) = 0;
};
typedef boost::shared_ptr<Surface> SurfaceHandle;

}
}

#endif
