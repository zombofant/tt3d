#ifndef _TT3D_GEOMETRY_COLOURS_H
#define _TT3D_GEOMETRY_COLOURS_H

#include "Vectors.hpp"

namespace tt3d {
namespace Math {

Vector3 hsvtorgb(const Vector3 &hsv);
Vector4 hsvtorgba(const Vector3 &hsv, const VectorFloat alpha);
Vector4 hsvatorgba(const Vector4 &hsv);

}
}

#endif
