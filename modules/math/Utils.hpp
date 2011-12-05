/**********************************************************************
File name: Utils.hpp
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

VectorFloat interpolateNearest(const VectorFloat a, const VectorFloat b, const VectorFloat factor);
VectorFloat interpolateLinear(const VectorFloat a, const VectorFloat b, const VectorFloat factor);
VectorFloat interpolateCos(const VectorFloat a, const VectorFloat b, const VectorFloat factor);

}
}

#endif
