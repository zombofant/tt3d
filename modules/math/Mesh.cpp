/**********************************************************************
File name: Mesh.cpp
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
#include "Mesh.hpp"
#include "modules/utils/Exception.hpp"

namespace tt3d {
namespace Math {
    
using namespace tt3d;

static int childIndicies[4][4] = {
    {
        0, 1, 8, 7
    }, 
    {
        1, 2, 3, 8
    },
    {
        8, 3, 4, 5
    },
    {
        7, 8, 5, 6
    }
};

/* tt3d::Math::MeshTree */

MeshTree::MeshTree():
    _parent(0),
    _position(TP_ROOT)
{
    
}

MeshTree::MeshTree(MeshTree *parent, const MeshTreePosition position):
    _parent(parent),
    _position(position)
{
    if (_parent) {
        _heightCallback = _parent->getHeightCallback();
    }
}

MeshTree *MeshTree::getParentSiblingChild(const MeshTreeSibling parentSibling, const MeshTreePosition position) const 
{
    MeshTree *parentSiblingItem = _parent->getSibling(parentSibling);
    if ((!parentSiblingItem) || (parentSiblingItem->isLeaf()))
        return 0;
    return ((MeshTreeNode*)parentSiblingItem)->getChild(position);
}

MeshTree *MeshTree::getSibling(const MeshTreeSibling sibling) const {
    /* 0 -> north
     * 1 -> west
     * 2 -> south
     * 3 -> east */
    if (!_parent)
        return 0;
    MeshTreeNode *parent = ((MeshTreeNode*)_parent);
    switch (_position) {
        case TP_ROOT:
            return 0;
        case TP_NORTH_WEST: {
            switch (sibling) {
                case TS_NORTH: {
                    return getParentSiblingChild(TS_NORTH, TP_SOUTH_WEST);
                }
                
                case TS_WEST: {
                    return getParentSiblingChild(TS_WEST, TP_NORTH_EAST);
                }
                
                case TS_SOUTH: {
                    return parent->getChild(TP_SOUTH_WEST);
                }
                
                case TS_EAST: {
                    return parent->getChild(TP_NORTH_EAST);
                }
            }
            break; /* case TP_NORTH_WEST */
        }
        case TP_SOUTH_WEST: {
            switch (sibling) {
                case TS_NORTH: {
                    return parent->getChild(TP_NORTH_WEST);
                }
                
                case TS_WEST: {
                    return getParentSiblingChild(TS_WEST, TP_SOUTH_EAST);
                }
                
                case TS_SOUTH: {
                    return getParentSiblingChild(TS_SOUTH, TP_NORTH_WEST);
                }
                
                case TS_EAST: {
                    return parent->getChild(TP_SOUTH_EAST);
                }
            }
            break; /* case TP_SOUTH_WEST */
        }
        case TP_SOUTH_EAST: {
            switch (sibling) {
                case TS_NORTH: {
                    return parent->getChild(TP_NORTH_EAST);
                }
                
                case TS_WEST: {
                    return parent->getChild(TP_SOUTH_WEST);
                }
                
                case TS_SOUTH: {
                    return getParentSiblingChild(TS_SOUTH, TP_NORTH_EAST);
                }
                
                case TS_EAST: {
                    return getParentSiblingChild(TS_EAST, TP_SOUTH_WEST);
                }
            }
            break; /* case TP_SOUTH_EAST */
        }
        case TP_NORTH_EAST: {
            switch (sibling) {
                case TS_NORTH: {
                    return getParentSiblingChild(TS_NORTH, TP_SOUTH_EAST);
                }
                
                case TS_WEST: {
                    return parent->getChild(TP_NORTH_WEST);
                }
                
                case TS_SOUTH: {
                    return parent->getChild(TP_SOUTH_EAST);
                }
                
                case TS_EAST: {
                    return getParentSiblingChild(TS_EAST, TP_NORTH_WEST);
                }
            }
            break; /* case TP_SOUTH_EAST */
        }
    }
    return 0;
}

/* tt3d::Math::MeshTreeNode */

MeshTreeNode::MeshTreeNode(MeshTree *parent, const MeshTreePosition position, const Vector2 min, const Vector2 max,
    const VectorFloat heights[4]):
    
    MeshTree::MeshTree(parent, position),
    _min(min),
    _max(max)
{
    initChildren(heights);
}

MeshTreeNode::MeshTreeNode(const Vector2 min, const Vector2 max, HeightCallback heightCallback):
    MeshTree::MeshTree(),
    _min(min),
    _max(max)
{
    setHeightCallback(heightCallback);
    initChildren(0);
}

MeshTreeNode::~MeshTreeNode() {
    for (int i = 0; i < 4; i++) {
        delete _children[i];
    }
}

void MeshTreeNode::initChildren(const VectorFloat heights[4]) {
    /* 0 3 
     * 1 2 */
    const Vector2 diag = _max - _min;
    const Vector2 positions[9] = {
        _min,
        Vector2(_min.x, _min.y + diag.y / 2.0),
        Vector2(_min.x, _min.y + diag.y),
        Vector2(_min.x + diag.x / 2.0, _min.y + diag.y),
        _max,
        Vector2(_min.x + diag.x, _min.y + diag.y / 2.0),
        Vector2(_min.x + diag.x, _min.y),
        Vector2(_min.x + diag.x / 2.0, _min.y),
        _min + diag / 2.0
    };
    VectorFloat allHeights[9];
    
    if (_heightCallback.ok()) {
        for (int i = 0; i < 9; i++) {
            allHeights[i] = _heightCallback.call(positions[i]);
        }
    } else {
        allHeights[0] = heights[0];
        allHeights[1] = (heights[1] + heights[0]) / 2.0;
        allHeights[2] = heights[1];
        allHeights[3] = (heights[2] + heights[1]) / 2.0;
        allHeights[4] = heights[2];
        allHeights[5] = (heights[3] + heights[2]) / 2.0;
        allHeights[6] = heights[3];
        allHeights[7] = (heights[0] + heights[3]) / 2.0;
        allHeights[8] = (heights[0] + heights[1] + heights[2] + heights[3]) / 2.0;
    }
    for (int i = TP_NORTH_WEST; i <= TP_NORTH_EAST; i++) {
        const VectorFloat childHeights[4] = {
            allHeights[childIndicies[i][0]],
            allHeights[childIndicies[i][1]],
            allHeights[childIndicies[i][2]],
            allHeights[childIndicies[i][3]]
        };
        _children[i] = new MeshTreeFace(this, (MeshTreePosition)i, positions[childIndicies[i][0]], positions[childIndicies[i][2]], childHeights);
    }
}

void MeshTreeNode::rangeCheck(const int index) const {
    if ((index < 0) || (index >= 4)) {
        throw Utils::IndexError(index, 0, 3);
    }
}

MeshTree *MeshTreeNode::getChild(const int index) {
    rangeCheck(index);
    return _children[index];
}

const MeshTree *MeshTreeNode::getChild(const int index) const {
    rangeCheck(index);
    return _children[index];
}

/*void subdivideChild(const int index, const Vector2 relPosition) {
    rangeCheck(index);
    MeshTreeFace *face = dynamic_cast<MeshTreeFace*>(_children[index]);
    if (!face) {
        throw Exception("The given child is not a face.");
    }
    const Vector2 absolute = face->getAbsolute(relPosition);
    _children[index] = MeshTreeNode(face);
    delete face;
}*/

void MeshTreeNode::selectTriangles(const Vector2 min, const Vector2 max,
    std::list<Triangle*> *triangles) const
{
    if (_min.x > max.x || _min.y > max.y || _max.x < min.x || _max.y < min.y) {
        return;
    }
    for (unsigned int i = 0; i < 4; i++) {
        _children[i]->selectTriangles(min, max, triangles);
    }
}

void MeshTreeNode::subdivideChild(const int index) {
    rangeCheck(index);
    MeshTreeFace *face = dynamic_cast<MeshTreeFace*>(_children[index]);
    if (!face) {
        throw Utils::Exception("The given child is not a face.");
    }
    _children[index] = face->subdivide();
    delete face;
}


void MeshTreeNode::traceSiblingVertices(const MeshTreePosition posA, const MeshTreePosition posB, std::vector<const Vector3*> &target) const {
    const MeshTree *childA = getChild(posA);
    const MeshTree *childB = getChild(posB);
    
    if (childA->isLeaf()) {
        MeshTreeFace *faceA = (MeshTreeFace*)childA;
        //target.push_back(faceA->vertex(posA));
        target.push_back(faceA->vertex(posB));
    } else {
        ((MeshTreeNode*)childA)->traceSiblingVertices(posA, posB, target);
    }
    
    if (childB->isLeaf()) {
        MeshTreeFace *faceB = (MeshTreeFace*)childB;
        //target.push_back(faceB->vertex(posA));
        target.push_back(faceB->vertex(posB));
    } else {
        ((MeshTreeNode*)childB)->traceSiblingVertices(posA, posB, target);
    }
}

/* tt3d::Math::MeshTreeFace */

MeshTreeFace::MeshTreeFace(MeshTree *parent, const MeshTreePosition position, const Vector2 min, const Vector2 max, const VectorFloat heights[4]):
    MeshTree::MeshTree(parent, position),
    _vertices({
        Vector3(min.x, min.y, heights[0]),
        Vector3(min.x, max.y, heights[1]),
        Vector3(max.x, max.y, heights[2]),
        Vector3(max.x, min.y, heights[3])
    })
{
    
}

void MeshTreeFace::rangeCheck(const int index) { 
    if ((index < 0) || (index >= 4)) {
        throw Utils::IndexError(index, 0, 3);
    }
}

void MeshTreeFace::getAdditionalSiblingVertices(const MeshTreeSibling siblingPosition, std::vector<const Vector3*> &target) const 
{
    MeshTree *sibling = getSibling(siblingPosition);
    if (!sibling || sibling->isLeaf()) {
        MeshTreePosition thisChild;
        switch (siblingPosition) {
            case TS_NORTH: {
                thisChild = TP_NORTH_EAST;
                break;
            }
            case TS_WEST: {
                thisChild = TP_NORTH_WEST;
                break;
            }
            case TS_SOUTH: {
                thisChild = TP_SOUTH_WEST;
                break;
            }
            case TS_EAST: {
                thisChild = TP_SOUTH_EAST;
                break;
            }
        }
        target.push_back(&_vertices[thisChild]);
        return;
    }
    MeshTreeNode *node = (MeshTreeNode*)(sibling);
    MeshTreePosition childA, childB;
    switch (siblingPosition) {
        case TS_NORTH: {
            childA = TP_SOUTH_WEST;
            childB = TP_SOUTH_EAST;
            break;
        };
        case TS_WEST: {
            childA = TP_SOUTH_EAST;
            childB = TP_NORTH_EAST;
            break;
        };
        case TS_SOUTH: {
            childA = TP_NORTH_EAST;
            childB = TP_NORTH_WEST;
            break;
        };
        case TS_EAST: {
            childA = TP_NORTH_WEST;
            childB = TP_SOUTH_WEST;
            break;
        };
    }
    
    node->traceSiblingVertices(childA, childB, target);
}

Vector3 MeshTreeFace::getCenter() const {
    Vector3 result(_vertices[0]);
    for (int i = 1; i < 4; i++) {
        result += _vertices[i];
    }
    result /= 4.0;
    
    return result;
}

VectorFloat MeshTreeFace::getError() const {
    const Vector3 origin = _vertices[0];
    const Vector3 northEdge = _vertices[3] - _vertices[0];
    const Vector3 westEdge = _vertices[1] - _vertices[0];
    VectorFloat error = 0.;
    
    for (int xi = 0; xi < 3; xi++) {
        const VectorFloat x = 0.25 + xi * 0.25;
        for (int yi = 0; yi < 3; yi++) {
            const VectorFloat y = 0.25 + yi * 0.25;
            const Vector3 pos = origin + northEdge * x + westEdge * y;
            error += abs(pos.z - _heightCallback.call(pos.vec2()));
        }
    }
    return error;
}

Vector3 *MeshTreeFace::vertex(const int index) {
    rangeCheck(index);
    return &_vertices[index];
}

void MeshTreeFace::selectTriangles(const Vector2 min, const Vector2 max,
    std::list<Triangle*> *triangles) const
{
    if (_vertices[0].x > max.x || _vertices[0].y > max.y || _vertices[2].x < min.x || _vertices[2].y < min.y) {
        return;
    }
    std::vector<const Vector3*> *list = new std::vector<const Vector3*>();
    Vector3 center = getCenter();
    if (_heightCallback.ok()) {
        center.z = _heightCallback.call(center.vec2());
    }
    getAdditionalSiblingVertices(TS_EAST, *list);
    getAdditionalSiblingVertices(TS_SOUTH, *list);
    getAdditionalSiblingVertices(TS_WEST, *list);
    getAdditionalSiblingVertices(TS_NORTH, *list);
    
    if (list->size() == 4) {
        triangles->push_back(new Triangle((*list)[0], (*list)[1], (*list)[2]));
        triangles->push_back(new Triangle((*list)[0], (*list)[2], (*list)[3]));
    } else {
        for (unsigned int i = 1; i < list->size(); i++) {
            triangles->push_back(new Triangle(&center, (*list)[i-1], (*list)[i]));
        }
        triangles->push_back(new Triangle(&center, (*list)[list->size()-1], (*list)[0]));
    }
    /*        Vector3List *list = new Vector3List();
        Vector3 center = face->getCenter();
        VectorFloat centerZ = center.z / 20.0 + 0.5;
        list->push_back(&center);
        face->getAdditionalSiblingVertices(TS_EAST, *list);
        face->getAdditionalSiblingVertices(TS_SOUTH, *list);
        face->getAdditionalSiblingVertices(TS_WEST, *list);
        face->getAdditionalSiblingVertices(TS_NORTH, *list);
        if (list->size() == 5) {
            glBegin(GL_QUADS);
                for (unsigned int i = 1; i < list->size(); i++) {
                    Vector3 *vert = (*list)[i];
                    const VectorFloat z = vert->z / 20.0 + 0.5;
                    glColor4f(z, z, z, 1.0);
                    glVertex3dv(vert->as_array);
                }
            glEnd();
        } else {
            glBegin(GL_TRIANGLE_FAN);
                Vector3 *prev = 0;
                for (unsigned int i = 0; i < list->size(); i++) {
                    Vector3 *vert = (*list)[i];
                    if (vert == prev) {
                        continue;
                    }
                    const VectorFloat z = vert->z / 20.0 + 0.5;
                    glColor4f(z, z, z, 1.0);
                    glVertex3dv(vert->as_array);
                    prev = vert;
                }
                const Vector3 *vert = (*list)[1];
                const VectorFloat z = vert->z / 20.0 + 0.5;
                glColor4f(z, z, z, 1.0);
                glVertex3dv(vert->as_array);
            glEnd();
        }
        delete list;
*/
}

MeshTreeNode *MeshTreeFace::subdivide() {
    MeshTreeNode *node;
    if (_heightCallback.ok()) {
        node = new MeshTreeNode(_parent, _position, _vertices[0].vec2(), _vertices[2].vec2(), 0);
    } else {
        const VectorFloat heights[4] = {
            _vertices[0].z,
            _vertices[1].z,
            _vertices[2].z,
            _vertices[3].z
        };
        
        node = new MeshTreeNode(_parent, _position, _vertices[0].vec2(), _vertices[2].vec2(), heights);
    }
    return node;
}
    
}
}
