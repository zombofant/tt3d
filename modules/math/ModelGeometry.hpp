/**********************************************************************
File name: ModelGeometry.hpp
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
#ifndef _TT3D_MATH_MODEL_GEOMETRY_H
#define _TT3D_MATH_MODEL_GEOMETRY_H
#include "Vectors.hpp"

namespace tt3d {
namespace Math {
    
struct Line {
    Vector3 vertices[2];
    
    Line();
    Line(const Line &line);
    Line(const Vector3 a, const Vector3 b);
};
    
struct Triangle {
    Vector3 vertices[3];
    
    Triangle();
    Triangle(const Triangle &line);
    Triangle(const Vector3 a, const Vector3 b, const Vector3 c);
    Triangle(const Vector3 *a, const Vector3 *b, const Vector3 *c);
};

struct Quad {
    Vector3 vertices[4];
    
    Quad();
    Quad(const Quad &line);
    Quad(const Vector3 a, const Vector3 b, const Vector3 c, const Vector3 d);
};

}
}

#endif
