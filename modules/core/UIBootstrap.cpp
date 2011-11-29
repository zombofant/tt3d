#include "UIBootstrap.hpp"
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "modules/geometry/Vectors.hpp"

namespace tt3d {
namespace Core {
    
using namespace tt3d;

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

    SDL_WM_SetCaption("tt3d", NULL);

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
}

void TT3D::initUI() {
    _uiBuffer = new UI::GeometryBuffer();
    _uiBufferHandle = GenericGeometryBufferHandle(_uiBuffer);
    
    _uiMaterial = new UI::Material();
    _uiMaterialHandle = UI::MaterialHandle(_uiMaterial);
    
    _rootWidget = new InGame();
    _rootHandle = WidgetHandle(_rootWidget);
    
    _uiBackground = new Surface3x3(_uiMaterialHandle);
    _uiBackgroundHandle = SurfaceHandle(_uiBackground);
    
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
    glLoadIdentity();
    
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
