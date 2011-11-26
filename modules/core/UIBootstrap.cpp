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
    static float positions[4][2] = {
        {0., 0.},
        {0., 100.},
        {100., 100.},
        {100., 0.}
    };
    static float colours[4][4] = {
        {0., 0., 0., 1.},
        {1., 0., 0., 1.},
        {0., 1., 0., 1.},
        {0., 0., 1., 1.}
    };
    
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
    
    testGeometry = new GL::GeometryBuffer<float, 2, 4>(GL_STATIC_DRAW);
    testIndexBuffer = new GL::StaticIndexBuffer(GL_STATIC_DRAW);
    
    GL::VertexIndexListHandle vertices = testGeometry->allocateVertices(4);
    
    for (int i = 0; i < 4; i++) {
        testGeometry->setPosition(i, positions[i]);
        testGeometry->setColour(i, colours[i]);
    }
    
    testIndexHandle = testIndexBuffer->add(vertices);
    testIndexBuffer->unbind();
    testGeometry->flush();
    testGeometry->unbind();
}

void TT3D::freeGL() {
    testIndexHandle.reset();
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
    glLoadIdentity();
    
    float position[2];
    float colour[4];
    
    glPointSize(2.0);
    
    glDisable(GL_BLEND);
    testGeometry->readBack();
    testGeometry->unbind();
    glBegin(GL_QUADS);
        for (int i = 0; i < 4; i++) {
            testGeometry->getColour(i, colour);
            glColor4fv(colour);
            std::cout << colour[0] << " " << colour[1] << " " << colour[2] << " " << colour[3] << std::endl;
            testGeometry->getPosition(i, position);
            std::cout << position[0] << " " << position[1] << std::endl;
            glVertex2fv(position);
        }
    glEnd();
    
    glTranslatef(101.0, 0.0, 0.0);
    
    glDrawArrays(GL_QUADS, 0, 4);
    glTranslatef(101.0, 0.0, 0.0);
    
    
    testGeometry->bind();
    testIndexBuffer->bind();
    testIndexBuffer->readBack();
    testIndexBuffer->dump();
    testIndexBuffer->draw(GL_QUADS);
    testIndexBuffer->unbind();
    testGeometry->unbind();
    
    SDL_GL_SwapBuffers();
}

void TT3D::freeApp() {
    freeGL();
    freeSDL();
}

}
}
