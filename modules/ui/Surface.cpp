#include "Surface.hpp"

namespace tt3d {
namespace UI {

using namespace tt3d;
using namespace tt3d::GL;
using namespace tt3d::Math;

/* tt3d::UI::Material */

Material::Material():
    GL::Material(GenericGeometryBufferHandle(new GeometryBuffer(GL_DYNAMIC_DRAW)))
{
    
}
/* tt3d::UI::Surface */

Surface::Surface(const MaterialHandle material):
    _material(material)
{
    
}
    
}
}
