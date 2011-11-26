#ifndef _TT3D_GL_MATERIAL_H
#define _TT3D_GL_MATERIAL_H

#include "Base.hpp"
#include "GeometryBuffer.hpp"
#include "IndexBuffer.hpp"

namespace tt3d {
namespace GL {

class Material: public Struct {
    public:
        Material(GenericGeometryBufferHandle geometryBuffer);
    private:
        GenericGeometryBufferHandle _geometryBuffer;
        StaticIndexBufferHandle _staticIndexBuffer;
        StreamIndexBufferHandle _streamIndexBuffer;
        bool _boundAsStream;
    public:
        GenericGeometryBufferHandle getGeometryBuffer() const { return _geometryBuffer; }
        StaticIndexBufferHandle getStaticIndexBuffer() const { return _staticIndexBuffer; }
        StreamIndexBufferHandle getStreamIndexBuffer() const { return _streamIndexBuffer; }
    public:
        virtual void bind();
        virtual void bind(const bool useStream);
        void render(const GLenum mode);
        virtual void unbind();
};
typedef boost::shared_ptr<Material> MaterialHandle;

}
}

#endif
