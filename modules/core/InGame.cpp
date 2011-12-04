
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
    _debugBuffer(new DebugGeometryBuffer()),
    _debugMaterial(new GL::Material(_debugBuffer)),
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
    DebugGeometryBuffer *buffer;
    Utils::BufferMapHandle mapHandle;
    Utils::BufferMap *map;
    GL::getMappedBuffer<DebugGeometryBuffer>(_axis, bufferHandle, buffer, mapHandle, map);
    
    map->setOffset(0);
    
    buffer->setPosition(0, Vector3(-0.1, 0.0, 0.0));
    buffer->setColour(0, Vector4(0.1, 0.0, 0.0, 1.0));
    buffer->setPosition(1, Vector3(1.0, 0.0, 0.0));
    buffer->setColour(1, Vector4(1.0, 0.0, 0.0, 1.0));

    buffer->setPosition(2, Vector3(0.0, -0.1, 0.0));
    buffer->setColour(2, Vector4(0.0, 0.1, 0.0, 1.0));
    buffer->setPosition(3, Vector3(0.0, 1.0, 0.0));
    buffer->setColour(3, Vector4(0.0, 1.0, 0.0, 1.0));

    buffer->setPosition(4, Vector3(0.0, 0.0, -0.1));
    buffer->setColour(4, Vector4(0.0, 0.0, 0.1, 1.0));
    buffer->setPosition(5, Vector3(0.0, 0.0, 1.0));
    buffer->setColour(5, Vector4(0.0, 0.0, 1.0, 1.0));
    
    buffer->setMap(Utils::BufferMapHandle());
}

void InGame::initCamera() {
    _camera->setFieldOfView(60.);
    _camera->setNearZ(1.0);
    _camera->setFarZ(1000.0);
    _camera->setViewport(_viewport);
}

void InGame::initGrid() {
    GL::GenericGeometryBufferHandle bufferHandle;
    DebugGeometryBuffer *buffer;
    Utils::BufferMapHandle mapHandle;
    Utils::BufferMap *map;
    GL::getMappedBuffer<DebugGeometryBuffer>(_grid, bufferHandle, buffer, mapHandle, map);
    
    const Vector4 gridColour(0.25, 0.25, 0.25, 0.75);
    
    map->setOffset(0);
    
    for (int i = -10; i <= 10; i++) {
        buffer->setPosition((i + 10)*2,         Vector3(i, -10.0, 0.0));
        buffer->setColour(  (i + 10)*2,         gridColour);
        buffer->setPosition((i + 10)*2+1,       Vector3(i,  10.0, 0.0));
        buffer->setColour(  (i + 10)*2+1,       gridColour);
    }
    
    for (int i = -10; i <= 10; i++) {
        buffer->setPosition((i + 31)*2,         Vector3(-10.0,  i, 0.0));
        buffer->setColour(  (i + 31)*2,         gridColour);
        buffer->setPosition((i + 31)*2+1,       Vector3( 10.0,  i, 0.0));
        buffer->setColour(  (i + 31)*2+1,       gridColour);
    }
    
    buffer->setMap(Utils::BufferMapHandle());
}

void InGame::initTest() {
    static int terrainSize = 256;
    IO::log << IO::ML_INFO << "Generating terrain (" << terrainSize << "×" << terrainSize << ")." << IO::submit;
    Terrain::SourceHandle source = Terrain::SourceHandle(new Terrain::PerlinNoiseSource(
        terrainSize, terrainSize, 
        Vector3(terrainSize, terrainSize, -6.0),
        Vector3(1., 1., 12.0),
        0.6, 8));
    IO::log << IO::ML_INFO << "Perlin initialized." << IO::submit;
    _mesh = new Terrain::TerrainMesh(source, Vector2(terrainSize, terrainSize), 1e-6, 8);
    IO::log << IO::ML_INFO << "Terrain generated." << IO::submit;
}

void InGame::doAbsRectChanged() {
    _viewport->setRect(_absRect);
    _camera->viewportChanged();
}

void InGame::doRenderCallback() {
    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    
    _camera->load();
    
    _debugMaterial->bind(false);
    _debugMaterial->render(GL_LINES);
    _debugMaterial->unbind();
    
    /*glColor4f(1.0, 1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
        glVertex3f(0.0, 0.0, 1.0);
        glVertex3f(0.0, 1.0, 2.0);
        glVertex3f(1.0, 1.0, 3.0);
        glVertex3f(1.0, 0.0, 4.0);
    glEnd();*/
    _mesh->debugRender();
    
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
