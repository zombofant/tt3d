#include "modules/math/Vectors.hpp"
#include "modules/math/Mesh.hpp"
#include "generators/Source.hpp"

namespace tt3d {
namespace Terrain {
    
using namespace tt3d;
using namespace Math;

class TerrainMesh {
    public:
        TerrainMesh(const SourceHandle source, 
            const Vector2 dimensions,
            const VectorFloat epsilon = 0.1);
    private:
        const Vector2 _dimensions;
        MeshTree *_mesh;
    protected:
        void buildMesh(const SourceHandle source, const VectorFloat epsilon);
    public:
        MeshTree *getMesh();
};

}
}
