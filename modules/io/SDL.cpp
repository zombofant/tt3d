/**********************************************************************
File name: SDL.cpp
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
#include <SDL/SDL.h>
#include "SDL.hpp"
#include <exception>
#include <iostream>

namespace tt3d {
namespace IO {
    
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
        default:;
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

void Application::perFrame(const TimeFloat interval) {
    
}

void Application::perSkippedFrame() {
    
}

void Application::runApp() {
    try {
        initApp();
    } catch (std::exception e) {
        std::cerr << e.what() << " during initialization." << std::endl;
        throw;
    } catch (...) {
        std::cerr << "unknown exception during initialization." << std::endl;
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
            const TimeFloat interval = timeIntervalToDouble(lastUpdate, currentUpdate);
            const TimeFloat fpsInterval = timeIntervalToDouble(frameCounterStart, currentUpdate);
            
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
        std::cerr << e.what() << " during runtime." << std::endl;
        freeApp();
        throw;
    } catch (...) {
        std::cerr << "unknown exception during runtime." << std::endl;
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
