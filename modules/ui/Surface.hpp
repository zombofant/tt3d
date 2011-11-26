#include "modules/geometry/Rect.hpp"
#include "modules/gl/GeometryObject.hpp"
#include "modules/gl/Material.hpp"

namespace tt3d {
namespace UI {

using namespace tt3d;
using namespace tt3d::GL;
using namespace tt3d::Geometry;

/*class GeometryBuffer: public GL::GeometryBuffer<float, 2, 4, 2> {
    
};*/
typedef GL::GeometryBuffer<float, 2, 4, 2> GeometryBuffer;

class Material: public GL::Material {
    public:
        Material();
};
typedef boost::shared_ptr<Material> MaterialHandle;

class GeometryObject: public GL::GeometryObject {
    public:
        GeometryObject(const GLsizei vertexCount, const MaterialHandle material);
};

typedef boost::shared_ptr<GeometryObject> GeometryObjectHandle;

class Surface {
    public:
        Surface(const MaterialHandle material);
    private:
        const MaterialHandle _material;
    public:
        virtual void updateGeometry(const Rect rect, GeometryObjectHandle geometry) = 0;
};
typedef boost::shared_ptr<Surface> SurfaceHandle;

}
}
