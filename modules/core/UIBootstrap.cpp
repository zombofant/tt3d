/**********************************************************************
File name: UIBootstrap.cpp
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
#include <fstream>
#include "UIBootstrap.hpp"
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "modules/math/Vectors.hpp"
#include <iostream>
#include "modules/math/Colours.hpp"
#include <cmath>
#include "modules/io/Log.hpp"

namespace tt3d {
namespace Core {
    
using namespace tt3d;

static inline double min(const double a, const double b) {
    return ((a<b)?a:b);
}

/* tt3d::Core::TT3D */

void TT3D::initIO() {
    IO::OStreamHandle plainLogStream = IO::OStreamHandle(new std::ofstream("log.txt"));
    IO::OStreamHandle xmlLogStream = IO::OStreamHandle(new std::ofstream("log.xml"));
    IO::OStreamHandle cout = IO::OStreamHandle(&std::cout);
    IO::OStreamHandle cerr = IO::OStreamHandle(&std::cerr);
    IO::log.addLogTarget(new IO::LogOStreamTarget(cout, (1<<IO::ML_HINT) | (1<<IO::ML_INFO)));
    IO::log.addLogTarget(new IO::LogOStreamTarget(cerr, (1<<IO::ML_WARNING) | (1<<IO::ML_ERROR)));
    IO::log.addLogTarget(new IO::LogOStreamTarget(plainLogStream));
    IO::log.addLogTarget(new IO::LogXMLFormatter(xmlLogStream, "log.xsl"));
    IO::log << IO::ML_INFO << "IO initialized." << IO::submit;
}

void TT3D::initSDL() {
    SDL_Init(SDL_INIT_VIDEO);
    
    const SDL_VideoInfo *info = SDL_GetVideoInfo();
    if (info == NULL) {
        IO::log << IO::ML_ERROR << "Could not get SDL video info." << IO::submit;
        SDL_Quit();
        throw 0;
        return;
    }
    
    uint32_t flags = SDL_OPENGL | SDL_HWPALETTE;
    if (info->hw_available != 0) {
        flags |= SDL_HWSURFACE;
    } else {
        flags |= SDL_SWSURFACE;
    }
    if (info->blit_hw != 0) {
        flags |= SDL_HWACCEL;
    }

    SDL_WM_SetCaption("tt3d", 0);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_SetVideoMode(1024, 768, 32, flags);
    if (window == NULL) {
        IO::log << IO::ML_FATAL << "Could not initialize SDL surface: " << SDL_GetError() << IO::submit;
        IO::log << IO::ML_FATAL << "Check your graphics driver." << IO::submit;
        SDL_Quit();
        throw 0;
        return;
    }

    SDL_EnableUNICODE(1);
    SDL_ShowCursor(1);
    
    IO::log << IO::ML_INFO << "SDL initialized." << IO::submit;
}

void TT3D::initGL() {    
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        IO::log << IO::ML_FATAL << "Could not initialize glew." << IO::submit;
        throw 0;
        return;
    }

    glClearColor(0.35, 0.5, 0.75, 1.0);
    glClearDepth(1.0);
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    globalViewport = ViewportHandle(new GL::Viewport(0, 0, 1024, 768));
    globalViewport->bind();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    IO::log << IO::ML_INFO << "OpenGL initialized." << IO::submit;
}

void TT3D::initUI() {
    const Vector4 baseColourHSV(M_PI*4/3, 0.5, 0.5, 1.0);
    const Vector4 activeColourHSV(M_PI*2/3, 0.5, 0.5, 1.0);
    
    _uiBuffer = new UI::GeometryBuffer();
    _uiBufferHandle = GenericGeometryBufferHandle(_uiBuffer);
    
    _uiMaterial = new UI::Material();
    _uiMaterialHandle = UI::MaterialHandle(_uiMaterial);
    
    _rootWidget = new InGame(globalViewport);
    _rootWidget->setAbsRect(Rect(0, 0, 1024, 768));
    _rootHandle = WidgetHandle(_rootWidget);
    
    _uiBackground = new Surface3x3(_uiMaterialHandle);
    _uiBackgroundHandle = SurfaceHandle(_uiBackground);
    
    const Vector4 baseColourHSV2 = Vector4(baseColourHSV.x, 0., baseColourHSV.z, 0.75);
    const Vector4 darkerColourRGB = hsvtorgba(Vector3(baseColourHSV2.x, 0., std::max(0.0, baseColourHSV2.z - 0.25)), 0.75);
    const Vector4 baseColourRGB = hsvatorgba(baseColourHSV2);
    
    _uiBackground->getQuad(0).setAllColours(Vector4(0., 0., 0., 0.));
    _uiBackground->getQuad(1).setAllColours(darkerColourRGB);
    _uiBackground->getQuad(1).colours[2] = baseColourRGB;
    _uiBackground->getQuad(2).setAllColours(baseColourRGB);
    _uiBackground->getQuad(2).colours[0] = darkerColourRGB;
    _uiBackground->getQuad(2).colours[3] = darkerColourRGB;
    _uiBackground->getQuad(3).setAllColours(baseColourRGB);
    _uiBackground->getQuad(4).setAllColours(baseColourRGB);
    _uiBackground->getQuad(5).setAllColours(baseColourRGB);
    _uiBackground->getQuad(6).setAllColours(baseColourRGB);
    _uiBackground->getQuad(7).setAllColours(baseColourRGB);
    _uiBackground->getQuad(8).setAllColours(baseColourRGB);
    _uiBackground->getQuad(8).colours[0] = darkerColourRGB;
    _uiBackground->getQuad(8).colours[1] = darkerColourRGB;
    
    _uiBackground->getBorders().top = 17;
    _uiBackground->getBorders().left = 28;
    _uiBackground->getBorders().right = 1;
    _uiBackground->getBorders().bottom = 1;
    
    _rootWidget->setSurface(_uiBackgroundHandle);
    IO::log << IO::ML_INFO << "UI initialized." << IO::submit;
}

void TT3D::handleKeypress(const SDL_keysym &sym, const SDL_KeyActionMode mode) {
    _rootWidget->deliverKeypress(sym, mode);
}

void TT3D::handleMouseButton(const SDL_MouseButtonEvent &button, const SDL_KeyActionMode mode) {
    _rootWidget->deliverMouseButton(button, mode);
}

void TT3D::handleMouseMotion(const SDL_MouseMotionEvent &motion) {
    _rootWidget->deliverMouseMotion(motion);
}

void TT3D::freeUI() {
    
}

void TT3D::freeGL() {
    
}

void TT3D::freeSDL() {
    SDL_FreeSurface(window);
    SDL_VideoQuit();
    SDL_Quit();
}

void TT3D::freeIO() {
    
}

void TT3D::initApp() {
    _updateTime = 0.;
    _updateInterval = 0.01;
    initIO();
    try {
        initSDL();
        try {
            initGL();
            try {
                initUI();
            } catch (...) {
                freeGL();
                throw;
            }
        } catch (...) {
            freeSDL();
            throw;
        }
    } catch (...) {
        freeIO();
        throw;
    }
}

void TT3D::doUpdate() {
    _rootWidget->update(_updateInterval);
}

void TT3D::perFrame(const IO::TimeFloat interval) {
    _updateTime += interval;
    while (_updateTime >= _updateInterval) {
        _updateTime -= _updateInterval;
        doUpdate();
    }
    
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    
    _rootWidget->render();
    
    SDL_GL_SwapBuffers();
}

void TT3D::freeApp() {
    freeUI();
    freeGL();
    freeSDL();
    freeIO();
}

}
}
