#include "coreUIBootstrap.hpp"
#include <SDL/SDL.h>
#include <GL/glew.h>

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
    
    globalViewport = new GL::Viewport(0, 0, 800, 600);
    globalViewport->bind();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    
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
    
    glBegin(GL_QUADS);
        glColor4f(1, 1, 1, 1);
        glVertex2f(0, 0);
        glColor4f(1, 0, 0, 1);
        glVertex2f(0, 128);
        glColor4f(0, 1, 0, 1);
        glVertex2f(128, 128);
        glColor4f(0, 0, 1, 1);
        glVertex2f(128, 0);
    glEnd();
    
    SDL_GL_SwapBuffers();
}

void TT3D::freeApp() {
    freeGL();
    freeSDL();
}

}
}
