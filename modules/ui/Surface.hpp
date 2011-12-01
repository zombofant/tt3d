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
