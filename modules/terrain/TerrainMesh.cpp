#include "modules/terrain/TerrainMesh.hpp"

namespace tt3d {
namespace Terrain {
    
VectorFloat heightCallback(void *userdata, const Vector2 pos) {
    return ((Source*)userdata)->getHeight(pos);
}

/* tt3d::Terrain::TerrainMesh */

TerrainMesh::TerrainMesh(const SourceHandle source, 
    const Vector2 dimensions,
    const VectorFloat epsilon):
    
    _dimensions(dimensions)
{
    buildMesh(source, epsilon);
}

void TerrainMesh::buildMesh(const SourceHandle source, const VectorFloat epsilon) {
    _mesh = new MeshTreeNode(Vector2(0., 0.), _dimensions, HeightCallback((void*)source.get(), &heightCallback));
    
}

}
}
