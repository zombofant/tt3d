#include "modules/terrain/TerrainMesh.hpp"
#include <cmath>

namespace tt3d {
namespace Terrain {
    
VectorFloat heightCallback(void *userdata, const Vector2 pos) {
    return ((Source*)userdata)->getHeight(pos);
}

/* tt3d::Terrain::TerrainMesh */

TerrainMesh::TerrainMesh(const SourceHandle source, 
    const Vector2 dimensions,
    const VectorFloat epsilon):
    _source(source),
    _dimensions(dimensions),
    _callback((void*)_source.get(), &heightCallback)
{
    buildMesh(epsilon);
}

void TerrainMesh::buildMesh(const VectorFloat epsilon) {
    _mesh = new MeshTreeNode(Vector2(0., 0.), _dimensions, _callback);
    recurseMesh(_mesh, epsilon);
}

void TerrainMesh::recurseMesh(MeshTreeNode *item, const VectorFloat epsilon) {
    for (int i = 0; i < 4; i++) {
        MeshTree *child = item->getChild(i);
        if (child->isLeaf()) {
            MeshTreeFace *face = (MeshTreeFace*)(child);
            if (face->getError() > epsilon) {
                item->subdivideChild(i);
                continue;
            }
        } else {
            MeshTreeNode *node = (MeshTreeNode*)(child);
            recurseMesh(node, epsilon);
        }
    }
}

}
}
