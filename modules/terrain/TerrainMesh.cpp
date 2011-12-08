/**********************************************************************
File name: TerrainMesh.cpp
This file is part of: tt3d — Freeform transport simulation

LICENSE

The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations under
the License.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public license (the  "GPL License"), in which case  the
provisions of GPL License are applicable instead of those above.

FEEDBACK & QUESTIONS

For feedback and questions about tt3d please e-mail one of the authors
named in the AUTHORS file.
**********************************************************************/
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
            const MeshTree *north = face->getSibling(TS_NORTH), 
                *south = face->getSibling(TS_SOUTH), 
                *east = face->getSibling(TS_EAST), 
                *west = face->getSibling(TS_WEST);
            if (error > epsilon || 
                ((!north || !north->isLeaf()) &&
                 (!south || !south->isLeaf()) &&
                 (!east || !east->isLeaf()) &&
                 (!west || !west->isLeaf()))
            ) {
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
    std::list<Triangle*> *triangles = new std::list<Triangle*>();
    /*_mesh->selectTriangles(Vector2(0, 0), Vector2(64, 64), triangles);
    _mesh->selectTriangles(Vector2(64, 0), Vector2(128, 64), triangles);
    _mesh->selectTriangles(Vector2(0, 64), Vector2(64, 128), triangles);
    _mesh->selectTriangles(Vector2(64, 64), Vector2(128, 128), triangles);*/
    _mesh->selectTriangles(Vector2(0, 0), _dimensions, triangles);
    glEnable(GL_CULL_FACE);
    glBegin(GL_TRIANGLES);
        Vector4f colour;
        for (std::list<Triangle*>::iterator it = triangles->begin();
            it != triangles->end();
            it++)
        {
            Triangle *triangle = *it;
            
            for (unsigned int i = 0; i < 3; i++) {
                Vector3 *vertex = &triangle->vertices[i];
                const VectorFloat z = vertex->z / 24.0 + 0.5;
                colour.x = z;
                colour.y = z;
                colour.z = z;
                colour.w = 1.0;
                
                glColor4fv(&colour.x);
                glVertex3dv(vertex->as_array);
            }
            
            delete triangle;
        }
    glEnd();
    //glBegin(GL_QUADS);
    //    debugRenderRecurse(_mesh);
    //glEnd();
    raiseLastGLError();
    delete triangles;
}

void TerrainMesh::writeToGeometryBuffer(TerrainGeometryBufferHandle buffer,
    const Vector2 min, const Vector2 max) const
{
    
}

}
}
