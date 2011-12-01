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
            const VectorFloat persistence, const unsigned int octaves);
    private:
        const VectorFloat _width, _height;
        const Vector3 _offset, _scale;
        const VectorFloat _persistence;
        const unsigned int _octaves;
    protected:
        VectorFloat interpolatedNoise(const Vector2 &pos);
    public:
        virtual VectorFloat getHeight(const Vector2 &pos);
        virtual void getMetrics(VectorFloat &width, VectorFloat &height);
};

}
}
