#include <SDL/SDL.h>
#include "SDL.hpp"
#include <exception>
#include <iostream>

namespace tt3d {
namespace SDL {
    
/* free functions */

timespec nanotime() {
    timespec result;
    clock_gettime(CLOCK_MONOTONIC_RAW, &result);
    return result;
}

double timeIntervalToDouble(const timespec &prev, const timespec &curr) {
    double result = (curr.tv_sec - prev.tv_sec);
    result += double(curr.tv_nsec - prev.tv_nsec) / (1000000000.);
    return result;
}

/* tt3d::SDL::Application */

Application::Application():
    terminated(false),
    frameCount(0)
{
    
}

Application::~Application() {
    
}

void Application::handleKeypress(const SDL_keysym &sym, const SDL_KeyActionMode mode) {
    switch (sym.sym) {
        case SDLK_q:
            if (sym.mod & KMOD_CTRL) 
                handleQuit();
            break;
    }
}

void Application::handleMouseButton(const SDL_MouseButtonEvent &button, const SDL_KeyActionMode mode) {
    
}

void Application::handleMouseMotion(const SDL_MouseMotionEvent &motion) {
    
}

void Application::handleUnhandledEvent(const SDL_Event &event) {
    
}

void Application::handleQuit() {
    terminate();
}

void Application::freeApp() {
    
}

void Application::initApp() {
    
}

void Application::perIteration() {
    
}

void Application::perFrame(const double interval) {
    
}

void Application::perSkippedFrame() {
    
}

void Application::runApp() {
    try {
        initApp();
    } catch (std::exception e) {
        std::cout << e.what() << " during initialization." << std::endl;
        throw;
    } catch (...) {
        std::cout << "unknown exception during initialization." << std::endl;
        throw;
    }
    
    try {
        lastUpdate = nanotime();
        frameCounterStart = lastUpdate;
        while (!terminated) {
            SDL_Event event;
            while (SDL_PollEvent(&event) == 1) {
                switch (event.type) {
                    case SDL_KEYDOWN:
                        handleKeypress(event.key.keysym, KM_PRESS);
                        break;
                    case SDL_KEYUP:
                        handleKeypress(event.key.keysym, KM_RELEASE);
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        handleMouseButton(event.button, KM_PRESS);
                        break;
                    case SDL_MOUSEBUTTONUP:
                        handleMouseButton(event.button, KM_RELEASE);
                        break;
                    case SDL_MOUSEMOTION:
                        handleMouseMotion(event.motion);
                        break;
                    case SDL_QUIT:
                        handleQuit();
                        break;
                    default:
                        handleUnhandledEvent(event);
                }
            }
            perIteration();
            timespec currentUpdate = nanotime();
            const double interval = timeIntervalToDouble(lastUpdate, currentUpdate);
            const double fpsInterval = timeIntervalToDouble(frameCounterStart, currentUpdate);
            
            if (fpsInterval >= 1.0) {
                currentFPS = (double)(frameCount) / fpsInterval;
                frameCounterStart = currentUpdate;
            }
            
            if (interval >= 0.01) {
                lastUpdate = currentUpdate;
                perFrame(interval);
                frameCount++;
            } else {
                perSkippedFrame();
            }
            
            usleep(1);
        }
    } catch (std::exception e) {
        std::cout << e.what() << " during runtime." << std::endl;
        freeApp();
        throw;
    } catch (...) {
        std::cout << "unknown exception during runtime." << std::endl;
        freeApp();
        throw;
    }
    freeApp();
}

void Application::terminate() {
    terminated = true;
}

}
}
