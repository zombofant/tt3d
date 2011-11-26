#include "modules/geometry/Rect.hpp"
#include "modules/gl/GeometryObject.hpp"
#include "modules/gl/Material.hpp"

namespace tt3d {
namespace UI {

using namespace tt3d;
using namespace tt3d::GL;
using namespace tt3d::Geometry;

class Material: public GL::Material {
    public:
        Material();
}

class Surface {
    public:
        virtual void updateGeometry(const Rect rect, 
            const Material material; GeometryObject *geometry);
}

}
}
