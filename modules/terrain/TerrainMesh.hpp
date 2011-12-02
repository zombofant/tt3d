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
            const VectorFloat epsilon = 0.1,
            const unsigned int maxLOD = 5);
        virtual ~TerrainMesh();
    private:
        const SourceHandle _source;
        const Vector2 _dimensions;
        const HeightCallback _callback;
        MeshTreeNode *_mesh;
    protected:
        void buildMesh(const VectorFloat epsilon, const unsigned int maxLOD);
        void debugRenderRecurse(MeshTree *node);
        bool recurseMesh(MeshTreeNode *node, const VectorFloat epsilon, const unsigned int currLOD, const unsigned int maxLOD);
    public:
        void debugRender();
    public:
        MeshTree *getMesh();
};

}
}
