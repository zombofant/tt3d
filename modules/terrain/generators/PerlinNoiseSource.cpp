#include "PerlinNoiseSource.hpp"
#include <cmath>
#include "modules/math/Utils.hpp"

namespace tt3d {
namespace Terrain {
    
using namespace tt3d;
using namespace Math;

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
    // assert(_octaves > 0);
}

VectorFloat PerlinNoiseSource::interpolatedNoise(const Vector2 &pos) {
    const int intX = floor(pos.x), intY = floor(pos.y);
    const VectorFloat fracX = abs(pos.x - VectorFloat(intX)), 
        fracY = abs(pos.y - VectorFloat(intY));
    
    /*std::cout << pos.x << std::endl;
    std::cout << intX << " " << fracX << std::endl;*/
    
    const double v[4] = {
        smoothedNoise(intX   ,   intY    ),
        smoothedNoise(intX+1 ,   intY    ),
        smoothedNoise(intX   ,   intY+1  ),
        smoothedNoise(intX+1 ,   intY+1  )
    };
    
    const double iv[2] = {
        interpolateCos(v[1], v[0], fracX),
        interpolateCos(v[3], v[2], fracX)
    };
    return interpolateCos(iv[1], iv[0], fracY);
}

VectorFloat PerlinNoiseSource::noise(const int x, const int y) {
    const int n = x + y * 57;
    const int m = (n << 13) xor n;
    return (1.0 - double((m * (m * m * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

VectorFloat PerlinNoiseSource::smoothedNoise(const int x, const int y) {
    static int xOffsets[8] = {-1, 1, -1, 1, -1, 1, 0, 0};
    static int yOffsets[8] = {-1, -1, 1, 1, 0, 0, -1, 1};
    double result = 0.;
    /*double intermediateResult = 0.;
    for (int i = 0; i < 4; i++) {
        intermediateResult += noise(x + xOffsets[i], y + yOffsets[i]);
    }
    result += intermediateResult / 16.0;
    
    intermediateResult = 0.;
    for (int i = 4; i < 8; i++) {
        intermediateResult += noise(x + xOffsets[i], y + yOffsets[i]);
    }
    result += intermediateResult / 8.0;*/
    return result + noise(x, y);
}

VectorFloat PerlinNoiseSource::getHeight(const Vector2 pos) {
    VectorFloat result = 0.0, frequency = 1./pow(2., _octaves), amplitude = _scale.z;
    Vector2 position(pos);
    position.x *= _scale.x;
    position.y *= _scale.y;
    position.x += _offset.x;
    position.y += _offset.y;
    
    for (unsigned int level = 0; level < _octaves; level++) {
        const Vector2 currPos(position * frequency);
        result += interpolatedNoise(currPos) * amplitude;
        frequency *= 2.0;
        amplitude *= _persistence;
    }
    
    return result;
}

void PerlinNoiseSource::getMetrics(VectorFloat &width, VectorFloat &height) {
    width = _width;
    height = _height;
}

}
}
