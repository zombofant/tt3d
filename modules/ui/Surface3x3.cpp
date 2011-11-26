#include "Surface3x3.hpp"
#include "modules/gl/GeometryQuadsForTris.hpp"

namespace tt3d {
namespace UI {

/* tt3d::UI::Surface3x3 */

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
    
    // Top left
    map->setOffset(0);
    buffer->setPosition(0, topLeft);
    buffer->setPosition(1, Vector2(topLeft.x, innerTL.y));
    buffer->setPosition(2, innerTL);
    buffer->setPosition(3, Vector2(innerTL.x, topLeft.y));
    _quads[0].apply(buffer);
    
    // Top center
    map->setOffset(4);
    buffer->setPosition(0, Vector2(innerTL.x, topLeft.y));
    buffer->setPosition(1, innerTL);
    buffer->setPosition(2, innerTR);
    buffer->setPosition(3, Vector2(innerTR.x, topLeft.y));
    _quads[1].apply(buffer);
    
    // Top right
    map->setOffset(8);
    buffer->setPosition(0, Vector2(innerTR.x, topLeft.y));
    buffer->setPosition(1, innerTR);
    buffer->setPosition(2, Vector2(innerTR.x + _rightMargin, innerTR.y));
    buffer->setPosition(3, Vector2(innerTR.x + _rightMargin, topLeft.y));
    _quads[2].apply(buffer);
    
    buffer->setMap(Utils::BufferMapHandle());
}

}
}
