#ifndef _TT3D_UI_SURFACE3X3_H
#define _TT3D_UI_SURFACE3X3_H

#include "modules/geometry/Vectors.hpp"
#include "Surface.hpp"

namespace tt3d {
namespace UI {
    
using namespace tt3d;
using namespace Geometry;

class Surface3x3: public Surface {
    public:
        Surface3x3(const MaterialHandle material);
    public:
        struct QuadInfo {
            Vector4 colours[4];
            Vector2 texCoords[4];
            
            QuadInfo();
            
            void apply(GeometryBuffer *mappedBuffer) const;
            void setAllColours(const Vector4 &colour);
            void setAllTexCoords(const Vector2 &texCoord);
        };
    protected:
        QuadInfo _quads[9];
        VectorFloat _leftMargin, _topMargin, _rightMargin, _bottomMargin;
    public:
        QuadInfo &getQuad(const int index) { return _quads[index]; };
        virtual void updateGeometry(const Rect rect, GeometryObjectHandle &geometry);
};

}
}

#endif
