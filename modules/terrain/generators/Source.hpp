#include "modules/math/Vectors.hpp"
#include <boost/smart_ptr/shared_ptr.hpp>

namespace tt3d {
namespace Terrain {
    
using namespace tt3d::Math;

class Source {
    public:
        virtual VectorFloat getHeight(const Vector2 &pos) = 0;
        virtual void getMetrics(VectorFloat &width, VectorFloat &height) = 0;
};

typedef boost::shared_ptr<Source> SourceHandle;

}
}
