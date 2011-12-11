/**********************************************************************
File name: Source.cpp
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
#include "Source.hpp"

namespace tt3d {
namespace Terrain {
    
/* tt3d::Terrain::Source */

void Source::getTangents(const Vector2 pos, const VectorFloat ds, Vector3 &tangent, Vector3 &bitangent) 
{
    const Vector2 posNX = pos + Vector2(-ds, 0.);
    const Vector2 posPX = pos + Vector2(ds, 0.);
    const Vector2 posNY = pos + Vector2(0., -ds);
    const Vector2 posPY = pos + Vector2(0., ds);
    
    const Vector3 vecNX(posNX, getHeight(posNX));
    const Vector3 vecPX(posPX, getHeight(posPX));
    const Vector3 vecNY(posNY, getHeight(posNY));
    const Vector3 vecPY(posPY, getHeight(posPY));
    
    tangent = (vecPX - vecNX) / (2.0 * ds);
    
    bitangent = (vecPY - vecNY) / (2.0 * ds);
}
    
}
}
