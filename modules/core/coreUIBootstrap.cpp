#include "coreUIBootstrap.hpp"
#include <SDL/SDL.h>

namespace tt3d {
namespace Core {

/* tt3d::Core::TT3D */

void TT3D::initSDL() {
    SDL_Init(SDL_INIT_VIDEO);
    
    const SDL_VideoInfo *info = SDL_GetVideoInfo();
    if (info == NULL) {
        SDL_Quit();
        terminate();
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
        terminate();
        return;
    }
    
    SDL_EnableUNICODE(1);
    SDL_ShowCursor(1);
}

void TT3D::initGL() {
    
}

void TT3D::freeGL() {
    
}

void TT3D::freeSDL() {
    SDL_Quit();
}

void TT3D::initApp() {
    initSDL();
    initGL();
}

void TT3D::perFrame(const double interval) {
    
}

void TT3D::freeApp() {
    freeGL();
    freeSDL();
}

}
}
