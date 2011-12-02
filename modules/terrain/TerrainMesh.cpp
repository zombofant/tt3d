#include "modules/terrain/TerrainMesh.hpp"
#include <cmath>
#include <GL/glew.h>
#include "modules/gl/Base.hpp"

namespace tt3d {
namespace Terrain {
    
using namespace GL;
    
VectorFloat heightCallback(void *userdata, const Vector2 pos) {
    return ((Source*)userdata)->getHeight(pos);
}

/* tt3d::Terrain::TerrainMesh */

TerrainMesh::TerrainMesh(const SourceHandle source, 
    const Vector2 dimensions,
    const VectorFloat epsilon,
    const unsigned int maxLOD):
    _source(source),
    _dimensions(dimensions),
    _callback((void*)_source.get(), &heightCallback)
{
    buildMesh(epsilon, maxLOD);
}

TerrainMesh::~TerrainMesh() {
    delete _mesh;
}

void TerrainMesh::buildMesh(const VectorFloat epsilon, const unsigned int maxLOD) {
    _mesh = new MeshTreeNode(Vector2(0., 0.), _dimensions, _callback);
    while (recurseMesh(_mesh, epsilon, 0, maxLOD)) {
        
    }
}

void TerrainMesh::debugRenderRecurse(MeshTree *node) {
    if (node->isLeaf()) {
        MeshTreeFace *face = (MeshTreeFace*)(node);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 4; i++) {
            const Vector3 *vert = face->vertex(i);
            const VectorFloat z = vert->z / 10.0;
            const Vector3f vertf = vert->toVector3f();
            glColor4f(z, z, z, 1.0);
            glVertex3dv(vert->as_array);
        }
        glEnd();
    } else {
        MeshTreeNode *next = (MeshTreeNode*)(node);
        for (int i = 0; i < 4; i++) {
            debugRenderRecurse(next->getChild(i));
        }
    }
}

bool TerrainMesh::recurseMesh(MeshTreeNode *item, const VectorFloat epsilon, const unsigned int currLOD, const unsigned int maxLOD) {
    if (currLOD > maxLOD) {
        return false;
    }
    bool changed = false;
    for (int i = 0; i < 4; i++) {
        MeshTree *child = item->getChild(i);
        if (child->isLeaf()) {
            MeshTreeFace *face = (MeshTreeFace*)(child);
            const VectorFloat error = face->getError();
            if (error > epsilon) {
                item->subdivideChild(i);
                
                changed = true;
                continue;
            }
        } else {
            MeshTreeNode *node = (MeshTreeNode*)(child);
            const bool tmpChanged = recurseMesh(node, epsilon, currLOD + 1, maxLOD);
            changed = changed || tmpChanged;
        }
    }
    return changed;
}

void TerrainMesh::debugRender() {
    //glBegin(GL_QUADS);
        debugRenderRecurse(_mesh);
    //glEnd();
    raiseLastGLError();
}

}
}
