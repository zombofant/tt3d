/**********************************************************************
File name: ModelGeometry.cpp
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
#include "ModelGeometry.hpp"

namespace tt3d {
namespace Math {

/* tt3d::Math::Line */

Line::Line():
    vertices()
{
    
}

Line::Line(const Line &line):
    vertices(line.vertices)
{
    
}

Line::Line(const Vector3 a, const Vector3 b):
    vertices{a, b}
{
    
}

/* tt3d::Math::Triangle */

Triangle::Triangle():
    vertices()
{
    
}

Triangle::Triangle(const Triangle &triangle):
    vertices(triangle.vertices)
{
    
}

Triangle::Triangle(const Vector3 a, const Vector3 b, const Vector3 c):
    vertices{a, b, c}
{
    
}

Triangle::Triangle(const Vector3 *a, const Vector3 *b, const Vector3 *c):
    vertices{*a, *b, *c}
{
    
}

/* tt3d::Math::Quad */

Quad::Quad():
    vertices()
{
    
}

Quad::Quad(const Quad &quad):
    vertices(quad.vertices)
{
    
}

Quad::Quad(const Vector3 a, const Vector3 b, const Vector3 c, const Vector3 d):
    vertices{a, b, c, d}
{
    
}

}
}
