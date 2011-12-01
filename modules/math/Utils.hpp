#ifndef _TT3D_MATH_UTILS_H
#define _TT3D_MATH_UTILS_H

#include "Vectors.hpp"
#include <cmath>

namespace tt3d {
namespace Math {
    
static inline VectorFloat sqr(const VectorFloat a) {
    return a*a;
}

static inline VectorFloat abs(const VectorFloat a) {
    return (a<0?-a:a);
}

static inline VectorFloat wrappingAbs(const VectorFloat a) {
    return (a<0?(1.0-abs(a)):abs(a));
}

/* free functions */

VectorFloat interpolateCos(const VectorFloat a, const VectorFloat b, const VectorFloat factor);

}
}

#endif
