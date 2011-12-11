/**********************************************************************
File name: InGame.cpp
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

#include "modules/io/Log.hpp"
#include "InGame.hpp"
#include <cmath>
#include <GL/glew.h>
#include "modules/terrain/generators/PerlinNoiseSource.hpp"

namespace tt3d {
namespace Core {
    
/* tt3d::Core::InGame */

using namespace tt3d;
using namespace tt3d::Math;

InGame::InGame(const GL::ViewportHandle viewport):
    UI::RootWidget(),
    _viewport(viewport),
    _debugVertexFormatHandle(new GL::VertexFormat(3, 4)),
    _debugBuffer(new DebugGeometryBuffer()),
    _debugMaterial(new GL::Material(_debugBuffer)),
    _terrainBuffer(new TerrainGeometryBuffer(GL_DYNAMIC_DRAW)),
    _terrainMaterial(new GL::Material(_terrainBuffer)),
    _axis(new GL::GeometryRaw(_debugMaterial, 6)),
    _grid(new GL::GeometryRaw(_debugMaterial, 84)),
    _camera(new GL::CameraFreeSmooth()),
    _cameraHandle(_camera)
{
    initCamera();
    initAxis();
    initGrid();
    initTest();
}

void InGame::initAxis() {
    GL::GenericGeometryBufferHandle bufferHandle;
    Utils::BufferMapHandle mapHandle;
    Utils::BufferMap *map;
    GL::GeometryBufferDriverHandle driverHandle;
    GL::getMappedBufferDriver(_debugVertexFormatHandle, _axis, bufferHandle, mapHandle, map, driverHandle);
    GL::GeometryBufferDriver *driver = driverHandle.get();
    
    map->setOffset(0);
    
    driver->setPosition(0, Vector3(-0.1, 0.0, 0.0));
    driver->setColour(0, Vector4(0.1, 0.0, 0.0, 1.0));
    driver->setPosition(1, Vector3(1.0, 0.0, 0.0));
    driver->setColour(1, Vector4(1.0, 0.0, 0.0, 1.0));

    driver->setPosition(2, Vector3(0.0, -0.1, 0.0));
    driver->setColour(2, Vector4(0.0, 0.1, 0.0, 1.0));
    driver->setPosition(3, Vector3(0.0, 1.0, 0.0));
    driver->setColour(3, Vector4(0.0, 1.0, 0.0, 1.0));

    driver->setPosition(4, Vector3(0.0, 0.0, -0.1));
    driver->setColour(4, Vector4(0.0, 0.0, 0.1, 1.0));
    driver->setPosition(5, Vector3(0.0, 0.0, 1.0));
    driver->setColour(5, Vector4(0.0, 0.0, 1.0, 1.0));
    
    bufferHandle->setMap(mapHandle);
    IO::log << IO::ML_INFO << "Initialized axis debug geometry." << IO::submit;
}

void InGame::initCamera() {
    _camera->setFieldOfView(60.);
    _camera->setNearZ(1.0);
    _camera->setFarZ(1000.0);
    _camera->setViewport(_viewport);
    IO::log << IO::ML_INFO << "Initialized camera." << IO::submit;
}

void InGame::initGrid() {
    GL::GenericGeometryBufferHandle bufferHandle;
    Utils::BufferMapHandle mapHandle;
    Utils::BufferMap *map;
    GL::GeometryBufferDriverHandle driverHandle;
    GL::getMappedBufferDriver(_debugVertexFormatHandle, _grid, bufferHandle, mapHandle, map, driverHandle);
    GL::GeometryBufferDriver *driver = driverHandle.get();
    
    const Vector4 gridColour(0.25, 0.25, 0.25, 0.75);
    
    map->setOffset(0);
    
    for (int i = -10; i <= 10; i++) {
        driver->setPosition((i + 10)*2,         Vector3(i, -10.0, 0.0));
        driver->setColour(  (i + 10)*2,         gridColour);
        driver->setPosition((i + 10)*2+1,       Vector3(i,  10.0, 0.0));
        driver->setColour(  (i + 10)*2+1,       gridColour);
    }
    
    for (int i = -10; i <= 10; i++) {
        driver->setPosition((i + 31)*2,         Vector3(-10.0,  i, 0.0));
        driver->setColour(  (i + 31)*2,         gridColour);
        driver->setPosition((i + 31)*2+1,       Vector3( 10.0,  i, 0.0));
        driver->setColour(  (i + 31)*2+1,       gridColour);
    }
    
    bufferHandle->setMap(Utils::BufferMapHandle());
    IO::log << IO::ML_INFO << "Initialized grid debug geometry." << IO::submit;
}

void InGame::initTest() {
    static int terrainSize = 64*8;
    IO::log << IO::ML_INFO << "Generating terrain (" << terrainSize << "×" << terrainSize << ")." << IO::submit;
    Terrain::SourceHandle source = Terrain::SourceHandle(new Terrain::PerlinNoiseSource(
        terrainSize, terrainSize, 
        Vector3(terrainSize, terrainSize, 0.0),
        Vector3(1., 1., 32.0),
        0.45, 7, 128.0));
    IO::log << IO::ML_INFO << "Perlin initialized." << IO::submit;
    _mesh = new Terrain::TerrainMesh(source, Vector2(terrainSize, terrainSize), 0.5, 32);
    IO::log << IO::ML_INFO << "Terrain generated, filling buffer" << IO::submit;
    _terrainObjectHandle = _mesh->createGeometryObject(_terrainMaterial, Vector2(0, 0), Vector2(terrainSize, terrainSize));
    IO::log << IO::ML_INFO << "Buffer filled." << IO::submit;
    
    _wireframe = false;
    _visualizeNormals = false;
}

void InGame::doAbsRectChanged() {
    _viewport->setRect(_absRect);
    _camera->viewportChanged();
}

void InGame::doRenderCallback() {
    static Vector4 ambient(0.1, 0.1, 0.1, 1.0);
    static Vector4 diffuse(1.0, 0.95, 0.9, 1.0);
    static Vector4 specular(0.0, 0.0, 0.0, 1.0);
    static Vector4 position(1.0, 1.0, 1.0, 0.);
    static Vector4 direction(-1.0, -1.0, -1.0, 0.0);
    position.normalize();
    
    
    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    
    glLoadIdentity();
    // glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, specular.toVector4f().as_array);
    
    _camera->load();
    glLightfv(GL_LIGHT0, GL_POSITION, position.toVector4f().as_array);
    
    _debugMaterial->bind(false);
    _debugMaterial->render(GL_LINES);
    _debugMaterial->unbind();
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient.toVector4f().as_array);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse.toVector4f().as_array);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular.toVector4f().as_array);
    
    glEnable(GL_COLOR_MATERIAL);
    /*glColor4f(1.0, 1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
        glVertex3f(0.0, 0.0, 1.0);
        glVertex3f(0.0, 1.0, 2.0);
        glVertex3f(1.0, 1.0, 3.0);
        glVertex3f(1.0, 0.0, 4.0);
    glEnd();*/
    if (_wireframe)
        glPolygonMode(GL_FRONT, GL_LINE);
    _terrainMaterial->bind(false);
    _terrainMaterial->render(GL_TRIANGLES);
    _terrainMaterial->unbind();
    if (_wireframe)
        glPolygonMode(GL_FRONT, GL_FILL);
    
    if (_visualizeNormals) {
        GL::GeometryBufferDriverHandle driverHandle = GL::GeometryBufferDriver::create(_terrainBuffer, Terrain::terrainVertexFormat);
        GL::GeometryBufferDriver *driver = driverHandle.get();
        Utils::BufferMapHandle mapHandle = _terrainObjectHandle->getMap();
        _terrainBuffer->setMap(mapHandle);
        mapHandle->setOffset(0);
        glColor4f(0.7, 0.7, 0.7, 1.0);
        glBegin(GL_LINES);
            for (int i = 0; i < _terrainObjectHandle->getVertexCount(); i++) 
            {
                Vector3f fPos, fNormal;
                driver->getPosition(i, fPos);
                driver->getNormal(i, fNormal);
                
                glColor3fv(fNormal.as_array);
                glVertex3fv(fPos.as_array);
                Vector3 pos2(fPos);
                pos2 += Vector3(fNormal) * 10.0;
                glVertex3dv(pos2.as_array);
            }
        glEnd();
    }
    // _terrainObjectHandle->drawDirect(GL_TRIANGLES);
    
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1024, 768, 0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void InGame::doUpdateCallback(const IO::TimeFloat interval) {
    _cameraMoved = false;
    _camera->update(interval);
}

void InGame::handleKeypress(const SDL_keysym &sym, const IO::SDL_KeyActionMode mode, bool &handled) {
    switch (sym.sym) {
        case SDLK_KP0: {
            if (mode == IO::KM_PRESS) {
                const Vector2 target = Vector2(0., 0.);
                _camera->issueMoveTo(target);
                _camera->issueRotTo(target);
            }
            handled = true;
            return;
        }
        case SDLK_TAB: {
            if (mode == IO::KM_PRESS) {
                _wireframe = !_wireframe;
            }
            handled = true;
            return;
        }
        default: {}
    }
    switch (sym.unicode) {
        case 'n': {
            if (mode == IO::KM_PRESS) {
                _visualizeNormals = !_visualizeNormals;
            }
            handled = true;
            return;
        }
        default: {}
    }
}

void InGame::handleMouseButton(const SDL_MouseButtonEvent &button, const IO::SDL_KeyActionMode mode) {
    switch (button.button) {
        case 4: {
            if (mode != IO::KM_PRESS)
                break;
            
            if ((SDL_GetKeyState(0)[SDLK_LCTRL] | SDL_GetKeyState(0)[SDLK_RCTRL]) != 0) {
                _camera->addAccelRot(Vector2(-4*M_PI, 0.));
            } else {
                _camera->addAccelZoom(-50.0 * _camera->getZoom());
            }
            break;
        }
        
        case 5: {
            if (mode != IO::KM_PRESS)
                break;
            
            if ((SDL_GetKeyState(0)[SDLK_LCTRL] | SDL_GetKeyState(0)[SDLK_RCTRL]) != 0) {
                _camera->addAccelRot(Vector2(4*M_PI, 0.));
            } else {
                _camera->addAccelZoom(50.0 * _camera->getZoom());
            }
            break;
        }
        
        case 6: {
            if (mode != IO::KM_PRESS)
                break;
            
            if ((SDL_GetKeyState(0)[SDLK_LCTRL] | SDL_GetKeyState(0)[SDLK_RCTRL]) != 0) {
                _camera->addAccel((_camera->getFlatRight() * 10. * _camera->getZoom() * 2.0).vec2());
            } else {
                _camera->addAccelRot(Vector2(0., -16*M_PI));
            }
            break;
        }
        
        case 7: {
            if (mode != IO::KM_PRESS)
                break;
            
            if ((SDL_GetKeyState(0)[SDLK_LCTRL] | SDL_GetKeyState(0)[SDLK_RCTRL]) != 0) {
                _camera->addAccel((-_camera->getFlatRight() * 10. * _camera->getZoom() * 2.0).vec2());
            } else {
                _camera->addAccelRot(Vector2(0., 16*M_PI));
            }
            break;
        }
        default: {}
    }
}

void InGame::handleMouseMotion(const SDL_MouseMotionEvent &motion) {
    if (((motion.state & SDL_BUTTON(2)) != 0) ||
        (((motion.state & SDL_BUTTON(1)) != 0) && ((motion.state & SDL_BUTTON(3)) != 0))) 
    {
        const VectorFloat factor = _camera->getZoom() * 2.0;
        Vector2 flatRight = _camera->getFlatRight().vec2();
        flatRight *= motion.xrel;
        Vector2 flatFront = _camera->getFlatFront().vec2();
        flatFront *= motion.yrel;
        Vector2 accel = flatRight + flatFront;
        accel *= factor;
        _camera->addAccel(accel);
        _camera->stopMove(false);
    } else if ((motion.state & SDL_BUTTON(3)) != 0) {
		if ((SDL_GetKeyState(0)[SDLK_LCTRL] | SDL_GetKeyState(0)[SDLK_RCTRL]) != 0) {
			_camera->addAccelZoom(-50. * motion.yrel * _camera->getZoom());
		} else {
			_camera->addAccelRot(Vector2(motion.yrel, motion.xrel) * M_PI);
			_camera->stopRot(false);
		}
    }
}

}
}
