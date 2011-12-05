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
#include "generators/Source.hpp"

namespace tt3d {
namespace Terrain {
    
using namespace tt3d;
using namespace Math;

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
        MeshTreeNode *_mesh;
    protected:
        void buildMesh(const VectorFloat epsilon, const unsigned int maxLOD);
        void debugRenderRecurse(MeshTree *node);
        bool recurseMesh(MeshTreeNode *node, const VectorFloat epsilon, const unsigned int currLOD, const unsigned int maxLOD);
    public:
        void debugRender();
    public:
        MeshTree *getMesh();
};

}
}
