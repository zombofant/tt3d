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

MeshTree::MeshTree(MeshTree *parent):
    _parent(parent),
    _heightCallback(parent->getHeightCallback())
{
    
}

/* tt3d::Math::MeshTreeNode */

MeshTreeNode::MeshTreeNode(MeshTree *parent, const Vector2 min, const Vector2 max,
    const VectorFloat heights[4]):
    
    MeshTree::MeshTree(parent),
    _min(min),
    _max(max)
{
    initChildren(heights);
}

MeshTreeNode::MeshTreeNode(const Vector2 min, const Vector2 max, HeightCallback heightCallback):
    MeshTree::MeshTree(0),
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
    for (int i = 0; i < 4; i++) {
        const VectorFloat childHeights[4] = {
            allHeights[childIndicies[i][0]],
            allHeights[childIndicies[i][1]],
            allHeights[childIndicies[i][2]],
            allHeights[childIndicies[i][3]]
        };
        _children[i] = new MeshTreeFace(this, positions[childIndicies[i][0]], positions[childIndicies[i][2]], childHeights);
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
    MeshTreeNode *node;
    if (_heightCallback.ok()) {
        node = new MeshTreeNode(_parent, _vertices[0].vec2(), _vertices[2].vec2(), 0);
    } else {
        const VectorFloat heights[4] = {
            _vertices[0].z,
            _vertices[1].z,
            _vertices[2].z,
            _vertices[3].z
        };
        
        node = new MeshTreeNode(_parent, _vertices[0].vec2(), _vertices[2].vec2(), heights);
    }
    return node;
}
    
}
}
