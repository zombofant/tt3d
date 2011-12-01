#include "UIBootstrap.hpp"
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "modules/math/Vectors.hpp"
#include <iostream>
#include "modules/math/Colours.hpp"
#include <cmath>

namespace tt3d {
namespace Core {
    
using namespace tt3d;

static inline double min(const double a, const double b) {
    return ((a<b)?a:b);
}

/* tt3d::Core::TT3D */

void TT3D::initIO() {
    
}

void TT3D::initSDL() {
    SDL_Init(SDL_INIT_VIDEO);
    
    const SDL_VideoInfo *info = SDL_GetVideoInfo();
    if (info == NULL) {
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

    window = SDL_SetVideoMode(800, 600, 32, flags);
    if (window == NULL) {
        SDL_Quit();
        throw 0;
        return;
    }

    SDL_EnableUNICODE(1);
    SDL_ShowCursor(1);
}

void TT3D::initGL() {    
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "glewInit failed." << std::endl;
        throw 0;
        return;
    }

    glClearColor(0.35, 0.5, 0.75, 1.0);
    glClearDepth(1.0);
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    globalViewport = ViewportHandle(new GL::Viewport(0, 0, 800, 600));
    globalViewport->bind();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void TT3D::initUI() {
    const Vector4 baseColourHSV(M_PI*4/3, 0.5, 0.5, 1.0);
    const Vector4 activeColourHSV(M_PI*2/3, 0.5, 0.5, 1.0);
    
    _uiBuffer = new UI::GeometryBuffer();
    _uiBufferHandle = GenericGeometryBufferHandle(_uiBuffer);
    
    _uiMaterial = new UI::Material();
    _uiMaterialHandle = UI::MaterialHandle(_uiMaterial);
    
    _rootWidget = new InGame(globalViewport);
    _rootWidget->setAbsRect(Rect(0, 0, 800, 600));
    _rootHandle = WidgetHandle(_rootWidget);
    
    _uiBackground = new Surface3x3(_uiMaterialHandle);
    _uiBackgroundHandle = SurfaceHandle(_uiBackground);
    
    const Vector4 baseColourHSV2 = Vector4(baseColourHSV.x, 0., baseColourHSV.z, 0.75);
    const Vector4 darkerColourRGB = hsvtorgba(Vector3(baseColourHSV2.x, 0., min(1.0, baseColourHSV2.z + 0.25)), 0.75);
    const Vector4 baseColourRGB = hsvatorgba(baseColourHSV2);
    
    _uiBackground->getQuad(0).setAllColours(Vector4(0., 0., 0., 0.));
    _uiBackground->getQuad(1).setAllColours(darkerColourRGB);
    _uiBackground->getQuad(2).setAllColours(baseColourRGB);
    _uiBackground->getQuad(2).colours[0] = darkerColourRGB;
    _uiBackground->getQuad(2).colours[2] = darkerColourRGB;
    _uiBackground->getQuad(3).setAllColours(baseColourRGB);
    _uiBackground->getQuad(8).setAllColours(baseColourRGB);
    _uiBackground->getQuad(8).colours[0] = darkerColourRGB;
    _uiBackground->getQuad(8).colours[1] = darkerColourRGB;
    _uiBackground->getQuad(7).setAllColours(baseColourRGB);
    
    _uiBackground->getBorders().top = 17;
    _uiBackground->getBorders().left = 28;
    
    std::cout << "ui bootstrapped" << std::endl;
    _rootWidget->setSurface(_uiBackgroundHandle);
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

void TT3D::perFrame(const double interval) {
    
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    
    _rootWidget->update(interval);
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
