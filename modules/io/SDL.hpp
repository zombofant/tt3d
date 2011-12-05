/**********************************************************************
File name: SDL.hpp
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
#ifndef _TT3D_IO_SDL_H
#define _TT3D_IO_SDL_H

#include <SDL/SDL.h>
#include "Time.hpp"

namespace tt3d {
namespace IO {
    
typedef enum {
    KM_PRESS,
    KM_RELEASE
} SDL_KeyActionMode;

class Application {
    public:
        Application();
        ~Application();
        
    private:
        bool terminated;
        timespec lastUpdate;
        uint32_t frameCount;
        timespec frameCounterStart;
        double currentFPS;
    
    protected:
        virtual void handleKeypress(const SDL_keysym &sym, const SDL_KeyActionMode mode);
        virtual void handleMouseButton(const SDL_MouseButtonEvent &button, const SDL_KeyActionMode mode);
        virtual void handleMouseMotion(const SDL_MouseMotionEvent &motion);
        virtual void handleUnhandledEvent(const SDL_Event &event);
        virtual void handleQuit();
        virtual void freeApp();
        virtual void initApp();
        virtual void perIteration();
        virtual void perFrame(const TimeFloat interval);
        virtual void perSkippedFrame();
    
    public:
        void runApp();
        void terminate();
};

}
}

#endif 
