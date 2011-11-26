#ifndef _TT3D_GL_GEOMETRY_OBJECT_H
#define _TT3D_GL_GEOMETRY_OBJECT_H

#include <GL/glew.h>
#include "IndexBuffer.hpp"
#include "Material.hpp"

namespace tt3d {
namespace GL {

class GeometryObject {
    public:
        GeometryObject(const GLsizei vertexCount, const MaterialHandle material);
    private:
        const MaterialHandle _material;
        VertexIndexListHandle _vertices;
        IndexEntryHandle _staticHandle;
        const GLsizei _vertexCount;
    public:
        void addToStreamBuffer();
        void addToStreamBuffer(StreamIndexBufferHandle indexBuffer);
        void drawDirect(const GLenum mode);
    public:
        GLsizei getCount() { return _vertexCount; }
};

}
}

#endif
