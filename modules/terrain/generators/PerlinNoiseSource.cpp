#include "PerlinNoiseSource.hpp"

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
    
}

VectorFloat PerlinNoiseSource::getHeight(const VectorFloat x, const VectorFloat y) {
    
}

void PerlinNoiseSource::getMetrics(VectorFloat &width, VectorFloat &height) {
    width = _width;
    height = _height;
}

}
}
