#include "modules/geometry/Vectors.hpp"

namespace tt3d {
namespace Terrain {
    
using namespace tt3d::Geometry;

class Source {
    public:
        virtual VectorFloat getHeight(const VectorFloat x, const VectorFloat y) = 0;
        virtual void getMetrics(VectorFloat &width, VectorFloat &height) = 0;
};

}
}
