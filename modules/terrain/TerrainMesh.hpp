/**********************************************************************
File name: TerrainMesh.hpp
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
#include "modules/math/Vectors.hpp"
#include "modules/math/Mesh.hpp"
#include "modules/math/ModelGeometry.hpp"
#include "modules/gl/GeometryBuffer.hpp"
#include "modules/gl/GeometryObject.hpp"
#include "modules/gl/Material.hpp"
#include "generators/Source.hpp"
#include "boost/shared_ptr.hpp"

namespace tt3d {
namespace Terrain {
    
using namespace tt3d;
using namespace tt3d::Math;
using namespace tt3d::GL;

//typedef GeometryBuffer<3, 4, 2, 0, 0, 0, true> TerrainGeometryBuffer;
//typedef boost::shared_ptr<TerrainGeometryBuffer> TerrainGeometryBufferHandle;
extern const VertexFormatHandle terrainVertexFormat;

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
        VectorFloat _currentError;
        unsigned int _faceCount;
        MeshTreeNode *_mesh;
    protected:
        void buildMesh(const VectorFloat epsilon, const unsigned int maxLOD);
        bool recurseMesh(MeshTreeNode *node, const VectorFloat epsilon, const unsigned int currLOD, const unsigned int maxLOD);
    public:
        void debugRender();
        void selectTriangles(const Vector2 min, const Vector2 max, std::vector<Triangle> *triangles) const;
        GeometryObjectHandle createGeometryObject(MaterialHandle material, const Vector2 min, const Vector2 max) const;
    public:
        MeshTree *getMesh();
};

}
}
