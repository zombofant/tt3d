/**********************************************************************
File name: Utils.cpp
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
#include "Utils.hpp"

namespace tt3d {
namespace Math {

/* free functions */

VectorFloat interpolateNearest(const VectorFloat a, const VectorFloat b, const VectorFloat factor) {
    return (factor>0.5?b:a);
}

VectorFloat interpolateLinear(const VectorFloat a, const VectorFloat b, const VectorFloat factor) {
    return (factor * a + (1.0 - factor) * b);
}

VectorFloat interpolateCos(const VectorFloat a, const VectorFloat b, const VectorFloat factor) {
    const double cosFactor = sqr(cos(factor * M_PI * 0.5));
    return (cosFactor * b + (1.0 - cosFactor) * a);
}

}
}
