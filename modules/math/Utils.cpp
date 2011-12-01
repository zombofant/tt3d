#include "Utils.hpp"

namespace tt3d {
namespace Math {

/* free functions */

VectorFloat interpolateLinear(const VectorFloat a, const VectorFloat b, const VectorFloat factor) {
    return (factor * a + (1.0 - factor) * b);
}

VectorFloat interpolateCos(const VectorFloat a, const VectorFloat b, const VectorFloat factor) {
    const double cosFactor = sqr(cos(factor * M_PI * 0.5));
    return (cosFactor * a + (1.0 - cosFactor) * b);
}

}
}
