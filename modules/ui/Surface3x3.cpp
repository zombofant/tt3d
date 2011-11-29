#include "Surface3x3.hpp"
#include "modules/gl/GeometryQuadsForTris.hpp"

namespace tt3d {
namespace UI {

/* tt3d::UI::Surface3x3 */

Surface3x3::Surface3x3(const MaterialHandle material):
    Surface::Surface(material),
    _quads({QuadInfo(), QuadInfo(), QuadInfo(), 
            QuadInfo(), QuadInfo(), QuadInfo(),
            QuadInfo(), QuadInfo(), QuadInfo()}),
    _leftMargin(0),
    _topMargin(0),
    _rightMargin(0),
    _bottomMargin(0)
{
    
}

void Surface3x3::updateGeometry(const Rect rect, GeometryObjectHandle &geometry) {
    if (!geometry.get()) {
        geometry = GeometryObjectHandle(new GeometryQuadsForTris(9, getMaterial()));
    }
    const Vector2 topLeft(rect.x, rect.y);
    const Vector2 innerTL = topLeft + Vector2(_leftMargin, _topMargin);
    const Vector2 innerTR = innerTL + Vector2(rect.w - (_leftMargin + _rightMargin), 0.);
    const Vector2 innerBL = innerTL + Vector2(0., rect.h - (_topMargin + _bottomMargin));
    const Vector2 innerBR = Vector2(innerTR.x, innerBL.y);
    
    const GenericGeometryBufferHandle bufferHandle = _material->getGeometryBuffer();
    GeometryBuffer *buffer = dynamic_cast<GeometryBuffer*>(bufferHandle.get());
    Utils::BufferMapHandle mapHandle = geometry->getMap();
    Utils::BufferMap *map = mapHandle.get();
    buffer->setMap(mapHandle);
    
    map->setOffset(0);
    buffer->setPosition(0, innerTL);
    buffer->setPosition(1, innerBL);
    buffer->setPosition(2, innerBR);
    buffer->setPosition(3, innerTR);
    _quads[0].apply(buffer);
    
    // Top left
    map->setOffset(4);
    buffer->setPosition(0, topLeft);
    buffer->setPosition(1, Vector2(topLeft.x, innerTL.y));
    buffer->setPosition(2, innerTL);
    buffer->setPosition(3, Vector2(innerTL.x, topLeft.y));
    _quads[1].apply(buffer);
    
    // Top center
    map->setOffset(8);
    buffer->setPosition(0, Vector2(innerTL.x, topLeft.y));
    buffer->setPosition(1, innerTL);
    buffer->setPosition(2, innerTR);
    buffer->setPosition(3, Vector2(innerTR.x, topLeft.y));
    _quads[2].apply(buffer);
    
    // Top right
    map->setOffset(12);
    buffer->setPosition(0, Vector2(innerTR.x, topLeft.y));
    buffer->setPosition(1, innerTR);
    buffer->setPosition(2, Vector2(innerTR.x + _rightMargin, innerTR.y));
    buffer->setPosition(3, Vector2(innerTR.x + _rightMargin, topLeft.y));
    _quads[3].apply(buffer);
    
    // Middle right
    map->setOffset(16);
    buffer->setPosition(0, innerTR);
    buffer->setPosition(1, innerBR);
    buffer->setPosition(2, Vector2(innerTR.x + _rightMargin, innerBR.y));
    buffer->setPosition(3, Vector2(innerTR.x + _rightMargin, innerTR.y));
    _quads[4].apply(buffer);
    
    // Bottom right
    map->setOffset(20);
    buffer->setPosition(0, innerBR);
    buffer->setPosition(1, Vector2(innerBR.x, innerBR.y + _bottomMargin));
    buffer->setPosition(2, Vector2(innerBR.x + _rightMargin, innerBR.y + _bottomMargin));
    buffer->setPosition(3, Vector2(innerBR.x + _rightMargin, innerBR.y));
    _quads[5].apply(buffer);
    
    // Bottom center
    map->setOffset(24);
    buffer->setPosition(0, innerBL);
    buffer->setPosition(1, Vector2(innerBL.x, innerBL.y + _bottomMargin));
    buffer->setPosition(2, Vector2(innerBR.x, innerBR.y + _bottomMargin));
    buffer->setPosition(3, innerBR);
    _quads[6].apply(buffer);
    
    // Bottom left
    map->setOffset(28);
    buffer->setPosition(0, Vector2(topLeft.x, innerBL.y));
    buffer->setPosition(1, Vector2(topLeft.x, innerBL.y + _bottomMargin));
    buffer->setPosition(2, Vector2(innerBL.x, innerBL.y + _bottomMargin));
    buffer->setPosition(3, innerBL);
    _quads[7].apply(buffer);
    
    // Middle left
    map->setOffset(32);
    buffer->setPosition(0, Vector2(topLeft.x, innerTL.y));
    buffer->setPosition(1, Vector2(topLeft.x, innerBL.y));
    buffer->setPosition(2, innerBL);
    buffer->setPosition(3, innerTL);
    _quads[8].apply(buffer);
    
    
    buffer->setMap(Utils::BufferMapHandle());
}

/* tt3d::UI::Surface3x3::QuadInfo */

Surface3x3::QuadInfo::QuadInfo():
    colours({Vector4(0., 0., 0., 1.), Vector4(0., 0., 0., 1.), Vector4(0., 0., 0., 1.), Vector4(0., 0., 0., 1.)}),
    texCoords({Vector2(0., 0.), Vector2(0., 0.), Vector2(0., 0.), Vector2(0., 0.)})
{
    
}

void Surface3x3::QuadInfo::apply(GeometryBuffer *mappedBuffer) const {
    for (int i = 0; i < 4; i++) {
        mappedBuffer->setColour(i, colours[i]);
        mappedBuffer->setTexCoord0(i, texCoords[i]);
    }
}


}
}
