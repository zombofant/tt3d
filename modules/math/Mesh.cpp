#include "Mesh.hpp"
#include "modules/utils/Exception.hpp"

namespace tt3d {
namespace Math {
    
using namespace tt3d;

/* tt3d::Math::MeshTree */

MeshTree::MeshTree(MeshTree *parent):
    _parent(parent)
{
    
}

/* tt3d::Math::MeshTreeNode */

MeshTreeNode::MeshTreeNode(MeshTree *parent, const Vector2 min, const Vector2 max,
    const VectorFloat heights[4]):
    
    MeshTree::MeshTree(parent),
    _min(min),
    _max(max)
{
    const Vector2 diag = max - min;
    const VectorFloat allHeights[9] = {
        /* 0 7 6
         * 1 8 5
         * 2 3 4 */
        
        heights[0],
        (heights[1] + heights[0]) / 2.0,
        heights[1],
        (heights[2] + heights[1]) / 2.0,
        heights[2],
        (heights[3] + heights[2]) / 2.0,
        heights[3],
        (heights[0] + heights[3]) / 2.0,
        (heights[0] + heights[1] + heights[2] + heights[3]) / 2.0
    };
    const VectorFloat childHeights[4][4] = {
        {
            allHeights[0],
            allHeights[1],
            allHeights[8],
            allHeights[7]
        }, 
        {
            allHeights[1],
            allHeights[2],
            allHeights[3],
            allHeights[8]
        },
        {
            allHeights[8],
            allHeights[3],
            allHeights[4],
            allHeights[5]
        },
        {
            allHeights[7],
            allHeights[8],
            allHeights[5],
            allHeights[6]
        }
    };
    const Vector2 childMinMax[4][2] = {
        {min, min + (diag / 2.0)},
        {Vector2(min.x, min.y + diag.y / 2.0), Vector2(min.x + diag.x / 2.0, min.y + diag.y)},
        {min + (diag / 2.0), max},
        {Vector2(min.x + diag.x / 2.0, min.y), Vector2(min.x + diag.x, min.y + diag.y / 2.0)}
    };
    for (int i = 0; i < 4; i++) {
        _children[i] = new MeshTreeFace(this, childMinMax[i][0], childMinMax[i][1], childHeights[i]);
    }
}

void MeshTreeNode::rangeCheck(const int index) {
    if ((index < 0) || (index >= 4)) {
        throw Utils::IndexError(index, 0, 3);
    }
}

MeshTree *MeshTreeNode::getChild(const int index) {
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

void MeshTreeNode::subdivideChild(const int index) {
    rangeCheck(index);
    MeshTreeFace *face = dynamic_cast<MeshTreeFace*>(_children[index]);
    if (!face) {
        throw Utils::Exception("The given child is not a face.");
    }
    _children[index] = face->subdivide();
    delete face;
}

/* tt3d::Math::MeshTreeFace */

MeshTreeFace::MeshTreeFace(MeshTree *parent, const Vector2 min, const Vector2 max, const VectorFloat heights[4]):
    MeshTree::MeshTree(parent),
    _vertices({
        Vector3(min.x, min.y, heights[0]),
        Vector3(min.x, max.y, heights[1]),
        Vector3(max.x, max.y, heights[2]),
        Vector3(max.x, min.y, heights[3])
    })
{
    
}

MeshTreeNode *MeshTreeFace::subdivide() {
    const VectorFloat heights[4] = {
        _vertices[0].z,
        _vertices[1].z,
        _vertices[2].z,
        _vertices[3].z
    };
    
    MeshTreeNode *node = new MeshTreeNode(_parent, _vertices[0].vec2(), _vertices[2].vec2(), heights);
    return node;
}
    
}
}
