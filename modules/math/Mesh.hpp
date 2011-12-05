/**********************************************************************
File name: Mesh.hpp
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
#include "Vectors.hpp"
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>

namespace tt3d {
namespace Math {

/** Vertex/child index map:
 *  
 *    north
 *  w 0  3  e
 *  e       a 
 *  s 1  2  s
 *  t south t
 * */
 
enum MeshTreePosition {
    TP_ROOT = -1,
    TP_NORTH_WEST = 0,
    TP_SOUTH_WEST = 1,
    TP_SOUTH_EAST = 2,
    TP_NORTH_EAST = 3
};

enum MeshTreeSibling {
    TS_NORTH = 0,
    TS_WEST = 1,
    TS_SOUTH = 2,
    TS_EAST = 3
};
    
class MeshTree;

typedef VectorFloat (HeightCallbackPtr)(void *userdata, const Vector2 pos);

struct HeightCallback {
    public:
        HeightCallback(void *aUserdata, HeightCallbackPtr *aCall):
            _userdata(aUserdata),
            _call(aCall) 
        {
            
        }
        
        HeightCallback():
            _userdata(0),
            _call(0) {};
    private:
        void *_userdata;
        HeightCallbackPtr *_call;
    public:
        VectorFloat call(const Vector2 pos) const { return _call(_userdata, pos); }
        bool ok() { return bool(_call); }
};

class MeshTree {
    public:
        virtual ~MeshTree() {};
    protected:
        MeshTree();
        MeshTree(MeshTree *parent, const MeshTreePosition position);
    protected:
        MeshTree *_parent;
        const MeshTreePosition _position;
        HeightCallback _heightCallback;
    protected:
        MeshTree *getParentSiblingChild(const MeshTreeSibling parentSibling, const MeshTreePosition position);
    public:
        HeightCallback getHeightCallback() { return _heightCallback; }
        MeshTree *getParent() { return _parent; }
        MeshTree *getSibling(const MeshTreeSibling sibling);
        
        virtual bool isLeaf() const { return false; }
        
        void setHeightCallback(HeightCallback heightCallback) { _heightCallback = heightCallback; }
};

class MeshTreeNode: public MeshTree {
    public:
        MeshTreeNode(MeshTree *parent, const MeshTreePosition position, const Vector2 min, const Vector2 max,
            const VectorFloat heights[4]);
        MeshTreeNode(const Vector2 min, const Vector2 max, HeightCallback heightCallback);
        virtual ~MeshTreeNode();
    private:
        const Vector2 _min, _max;
        MeshTree *_children[4];
    protected:
        void initChildren(const VectorFloat heights[4]);
        void rangeCheck(const int index);
    public:
        MeshTree *getChild(const int index);
        void subdivideChild(const int index);
        void traceSiblingVertices(const MeshTreePosition posA, const MeshTreePosition posB, Vector3List &target);
};

class MeshTreeFace: public MeshTree {
    public:
        MeshTreeFace(MeshTree *parent, const MeshTreePosition position, const Vector2 min, const Vector2 max,
            const VectorFloat heights[4]);
    private:
        Vector3 _vertices[4];
    protected:
        void rangeCheck(const int index);
    public:
        void getAdditionalSiblingVertices(const MeshTreeSibling siblingPosition, Vector3List &target);
        Vector3 getCenter() const;
        void getEdges(Vector3 &northEdge, Vector3 &westEdge, Vector3 &southEdge, Vector3 &eastEdge);
        VectorFloat getError() const;
        
        virtual bool isLeaf() const { return true; }
        
        Vector3 *vertex(const int index);
    public:
        MeshTreeNode *subdivide();
        
    friend class MeshTreeNode;
};

typedef boost::shared_ptr<MeshTree> MeshTreeHandle;

}
}
