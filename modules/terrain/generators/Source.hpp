#include "modules/math/Vectors.hpp"

namespace tt3d {
namespace Terrain {
    
using namespace tt3d::Math;

class Source {
    public:
        virtual VectorFloat getHeight(const Vector2 &pos) = 0;
        virtual void getMetrics(VectorFloat &width, VectorFloat &height) = 0;
};

}
}
