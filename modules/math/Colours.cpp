#include "Colours.hpp"
#include <cmath>

namespace tt3d {
namespace Math {

/* free functions */
Vector3 hsvtorgb(const Vector3 &hsv) {
    if (hsv.y == 0) {
        return Vector3(hsv.z, hsv.z, hsv.z);
    }
    
    Vector4 result;
    const double hTmp1 = ((hsv.x < 0)?(M_PI*2 - hsv.x):hsv.x);
    const double hTmp2 = hTmp1 - round(hTmp1 / (M_PI*2)) * M_PI*2.;
    const double hTmp3 = hTmp2 / (M_PI / 3);
    const int i = floor(hTmp3);
    
    const double f = hTmp3 - i;
    const double v = hsv.y;
    const double p = hsv.z * (1.0 - hsv.y);
    const double q = hsv.z * (1.0 - (hsv.y * f));
    const double t = hsv.z * (1.0 - (hsv.y * (1.0 - f)));
    
    switch (i) {
        case 0:
            return Vector3(v, t, p);
        case 1:
            return Vector3(q, v, p);
        case 2:
            return Vector3(p, v, t);
        case 3:
            return Vector3(p, q, v);
        case 4:
            return Vector3(t, p, v);
        case 5:
            return Vector3(v, p, q);
        default:
            return Vector3();
    }
}

Vector4 hsvtorgba(const Vector3 &hsv, const VectorFloat alpha) {
    return Vector4(hsvtorgb(hsv), alpha);
}

Vector4 hsvatorgba(const Vector4 &hsva) {
    return Vector4(hsvtorgb(hsva.vec3()), hsva.a);
}

}
}
