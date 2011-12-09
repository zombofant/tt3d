/**********************************************************************
File name: Surface3x3.cpp
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
#include "Surface3x3.hpp"
#include "modules/gl/GeometryQuadsForTris.hpp"
#include <iostream>

namespace tt3d {
namespace UI {

/* tt3d::UI::Surface3x3 */

Surface3x3::Surface3x3(const MaterialHandle material):
    Surface::Surface(material),
    _quads({QuadInfo(), QuadInfo(), QuadInfo(), 
            QuadInfo(), QuadInfo(), QuadInfo(),
            QuadInfo(), QuadInfo(), QuadInfo()}),
    _margin()
{
    
}

void Surface3x3::updateGeometry(const Rect rect, GeometryObjectHandle &geometry) {
    if (!geometry.get()) {
        geometry = GeometryObjectHandle(new GeometryQuadsForTris(9, getMaterial()));
    }
    const Vector2 topLeft(rect.x, rect.y);
    const Vector2 innerTL = topLeft + Vector2(_margin.left, _margin.top);
    const Vector2 innerTR = innerTL + Vector2(rect.w - (_margin.left + _margin.right), 0.);
    const Vector2 innerBL = innerTL + Vector2(0., rect.h - (_margin.top + _margin.bottom));
    const Vector2 innerBR = Vector2(innerTR.x, innerBL.y);
    
    const GenericGeometryBufferHandle bufferHandle = _material->getGeometryBuffer();
    GenericGeometryBuffer *buffer = bufferHandle.get();
    // GeometryBuffer *buffer = dynamic_cast<GeometryBuffer*>(bufferHandle.get());
    GeometryBufferDriverHandle driverHandle = GeometryBufferDriver::create(bufferHandle, surfaceVertexFormat);
    assert(driverHandle.get());
    GeometryBufferDriver *driver = driverHandle.get();
    Utils::BufferMapHandle mapHandle = geometry->getMap();
    Utils::BufferMap *map = mapHandle.get();
    buffer->setMap(mapHandle);
    
    // Center
    map->setOffset(0);
    driver->setPosition(0, innerTL);
    driver->setPosition(1, innerBL);
    driver->setPosition(2, innerBR);
    driver->setPosition(3, innerTR);
    _quads[0].apply(driver);
    
    // Top left
    map->setOffset(4);
    driver->setPosition(0, topLeft);
    driver->setPosition(1, Vector2(topLeft.x, innerTL.y));
    driver->setPosition(2, innerTL);
    driver->setPosition(3, Vector2(innerTL.x, topLeft.y));
    _quads[1].apply(driver);
    
    // Top center
    map->setOffset(8);
    driver->setPosition(0, Vector2(innerTL.x, topLeft.y));
    driver->setPosition(1, innerTL);
    driver->setPosition(2, innerTR);
    driver->setPosition(3, Vector2(innerTR.x, topLeft.y));
    _quads[2].apply(driver);
    
    // Top right
    map->setOffset(12);
    driver->setPosition(0, Vector2(innerTR.x, topLeft.y));
    driver->setPosition(1, innerTR);
    driver->setPosition(2, Vector2(innerTR.x + _margin.right, innerTR.y));
    driver->setPosition(3, Vector2(innerTR.x + _margin.right, topLeft.y));
    _quads[3].apply(driver);
    
    // Middle right
    map->setOffset(16);
    driver->setPosition(0, innerTR);
    driver->setPosition(1, innerBR);
    driver->setPosition(2, Vector2(innerBR.x + _margin.right, innerBR.y));
    driver->setPosition(3, Vector2(innerTR.x + _margin.right, innerTR.y));
    _quads[4].apply(driver);
    
    // Bottom right
    map->setOffset(20);
    driver->setPosition(0, innerBR);
    driver->setPosition(1, Vector2(innerBR.x, innerBR.y + _margin.bottom));
    driver->setPosition(2, Vector2(innerBR.x + _margin.right, innerBR.y + _margin.bottom));
    driver->setPosition(3, Vector2(innerBR.x + _margin.right, innerBR.y));
    _quads[5].apply(driver);
    
    // Bottom center
    map->setOffset(24);
    driver->setPosition(0, innerBL);
    driver->setPosition(1, Vector2(innerBL.x, innerBL.y + _margin.bottom));
    driver->setPosition(2, Vector2(innerBR.x, innerBR.y + _margin.bottom));
    driver->setPosition(3, innerBR);
    _quads[6].apply(driver);
    
    // Bottom left
    map->setOffset(28);
    driver->setPosition(0, Vector2(topLeft.x, innerBL.y));
    driver->setPosition(1, Vector2(topLeft.x, innerBL.y + _margin.bottom));
    driver->setPosition(2, Vector2(innerBL.x, innerBL.y + _margin.bottom));
    driver->setPosition(3, innerBL);
    _quads[7].apply(driver);
    
    // Middle left
    map->setOffset(32);
    driver->setPosition(0, Vector2(topLeft.x, innerTL.y));
    driver->setPosition(1, Vector2(topLeft.x, innerBL.y));
    driver->setPosition(2, innerBL);
    driver->setPosition(3, innerTL);
    _quads[8].apply(driver);
    
    
    buffer->setMap(Utils::BufferMapHandle());
    std::cout << "geometry is setup" << std::endl;
}

/* tt3d::UI::Surface3x3::QuadInfo */

Surface3x3::QuadInfo::QuadInfo():
    colours({Vector4(0., 0., 0., 1.), Vector4(0., 0., 0., 1.), Vector4(0., 0., 0., 1.), Vector4(0., 0., 0., 1.)}),
    texCoords({Vector2(0., 0.), Vector2(0., 0.), Vector2(0., 0.), Vector2(0., 0.)})
{
    
}

void Surface3x3::QuadInfo::apply(GeometryBufferDriver *bufferDriver) const {
    for (int i = 0; i < 4; i++) {
        bufferDriver->setColour(i, colours[i]);
        bufferDriver->setTexCoord0(i, texCoords[i]);
        /*mappedBuffer->setColour(i, Vector4(0.7, 0.7, 0.7, 0.5));
        mappedBuffer->setTexCoord0(i, Vector2(0., 0.));*/
    }
}

void Surface3x3::QuadInfo::setAllColours(const Vector4 &colour) {
    for (int i = 0; i < 4; i++) {
        colours[i] = colour;
    }
}

void Surface3x3::QuadInfo::setAllTexCoords(const Vector2 &texCoord) {
    for (int i = 0; i < 4; i++) {
        texCoords[i] = texCoord;
    }
}


}
}
