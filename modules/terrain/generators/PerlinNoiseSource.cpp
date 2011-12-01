#include "PerlinNoiseSource.hpp"
#include <cmath>

namespace tt3d {
namespace Terrain {

/* tt3d::Terrain::PerlinNoiseSource */

PerlinNoiseSource::PerlinNoiseSource(const VectorFloat width, const VectorFloat height, 
    const Vector3 offset, const Vector3 scale, 
    const VectorFloat persistence, const unsigned int octaves):
    
    _width(width),
    _height(height),
    _offset(offset),
    _scale(scale),
    _persistence(persistence),
    _octaves(octaves)
{
    assert(_octaves > 0);
}

VectorFloat interpolatedNoise(const Vector2 &pos) {
    const int intX = round(pos.x), intY = round(pos.y);
    const VectorFloat fracX = wrappedAbs(pos.x - intX), 
        fracY = wrappedAbs(pos.y - intY);
    
    const double v[3] = {;
        smoothedNoise(intX   ,   intY    ),
        smoothedNoise(intX+1 ,   intY    ),
        smoothedNoise(intX   ,   intY+1  ),
        smoothedNoise(intX+1 ,   intY+1  )
    }
    
    const double iv[2] = {
        interpolateCos(v[0], v[1], fracX),
        interpolateCos(v[2], v[3], fracX)
    }
    return interpolateCos(iv[0], iv[1], fracY);
}

VectorFloat PerlinNoiseSource::getHeight(const Vector2 pos) {
    VectorFloat result = 0.0, frequency = 1.0, amplitude = 1.0;
    Vector2 position(pos);
    pos.x *= _scale.x;
    pos.y *= _scale.y;
    pos.x += _offset.x;
    pos.y += _offset.y;
    
    for (unsigned int level = 0; level < _octaves; level++) {
        const Vector2 currPos(pos * frequency);
        result += interpolatedNoise(position);
        frequency *= 2.0;
        amplitude *= _persistence;
    }
}

void PerlinNoiseSource::getMetrics(VectorFloat &width, VectorFloat &height) {
    width = _width;
    height = _height;
}

}
}
