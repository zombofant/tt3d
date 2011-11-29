#ifndef _TT3D_GL_GEOMETRY_QUADSFORTRIS_H
#define _TT3D_GL_GEOMETRY_QUADSFORTRIS_H

#include "GeometryObject.hpp"

namespace tt3d {
namespace GL {
    
using namespace tt3d;

class GeometryQuadsForTris: public GeometryObject {
    public:        
        GeometryQuadsForTris(const GLsizei quadCount, const MaterialHandle material):
            GeometryObject(material),
            _quadCount(quadCount) {};
    private:
        const GLsizei _quadCount;
    protected:
        virtual VertexAllocationHandle allocateVertices();
};

}
}

#endif
