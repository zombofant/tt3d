/**********************************************************************
File name: PerlinNoiseSource.hpp
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
#ifndef _TT3D_TERRAIN_PERLIN_NOISE_H
#define _TT3D_TERRAIN_PERLIN_NOISE_H

#include "modules/math/Vectors.hpp"
#include "Source.hpp"

namespace tt3d {
namespace Terrain {
    
using namespace tt3d;
using namespace tt3d::Math;

class PerlinNoiseSource: public Source {
    public:
        PerlinNoiseSource(const VectorFloat width, const VectorFloat height, 
            const Vector3 offset, const Vector3 scale, 
            const VectorFloat persistence, const unsigned int octaves, 
            const VectorFloat largestFeature);
    private:
        const VectorFloat _width, _height;
        const Vector3 _offset, _scale;
        const VectorFloat _persistence;
        const unsigned int _octaves;
        const VectorFloat _largestFeature;
    protected:
        VectorFloat interpolatedNoise(const Vector2 &pos);
        VectorFloat noise(const int x, const int y);
        VectorFloat smoothedNoise(const int x, const int y);
    public:
        virtual VectorFloat getHeight(const Vector2 pos);
        virtual void getMetrics(VectorFloat &width, VectorFloat &height);
};

}
}

#endif
