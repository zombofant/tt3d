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
    
class MeshTree;

typedef VectorFloat (HeightCallbackPtr)(void *userdata, const Vector2 pos);

struct HeightCallback {
    public:
        HeightCallback(void *aUserdata, HeightCallbackPtr *aCall):
            _userdata(aUserdata),
            _call(aCall) 
        {
            
        }
    private:
        void *_userdata;
        HeightCallbackPtr *_call;
    public:
        VectorFloat call(const Vector2 pos) { return _call(_userdata, pos); }
        bool ok() { return bool(_call); }
};

class MeshTree {
    public:
        virtual ~MeshTree() {};
    protected:
        MeshTree(MeshTree *parent);
    protected:
        MeshTree *_parent;
        HeightCallback _heightCallback;
        virtual bool isLeaf() { return false; }
    public:
        HeightCallback getHeightCallback() { return _heightCallback; }
        MeshTree *getParent() { return _parent; }
        
        void setHeightCallback(HeightCallback heightCallback) { _heightCallback = heightCallback; }
};

class MeshTreeNode: public MeshTree {
    public:
        MeshTreeNode(MeshTree *parent, const Vector2 min, const Vector2 max,
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
};

class MeshTreeFace: public MeshTree {
    public:
        MeshTreeFace(MeshTree *parent, const Vector2 min, const Vector2 max,
            const VectorFloat heights[4]);
    private:
        Vector3 _vertices[4];
    protected:
        virtual bool isLeaf() { return true; }
    public:
        Vector2 getAbsolute(const Vector2 relPosition) const;
        Vector3 getCenter() const;
        void getEdges(Vector3 &northEdge, Vector3 &westEdge, Vector3 &southEdge, Vector3 &eastEdge);
        Vector3 *vertex(const int index);
    public:
        MeshTreeNode *subdivide();
        
    friend class MeshTreeNode;
};

typedef boost::shared_ptr<MeshTree> MeshTreeHandle;

}
}
