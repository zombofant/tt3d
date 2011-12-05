/**********************************************************************
File name: UIBootstrap.hpp
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
#ifndef _TT3D_CORE_UI_BOOTSTRAP_H
#define _TT3D_CORE_UI_BOOTSTRAP_H

#include <SDL/SDL.h>
#include "modules/io/SDL.hpp"
#include "modules/io/vfs/VFS.hpp"
#include "modules/io/vfs/MountDirectory.hpp"
#include "modules/gl/Viewport.hpp"
#include "modules/gl/GeometryBuffer.hpp"
#include "modules/gl/IndexBuffer.hpp"
#include "modules/ui/Surface.hpp"
#include "modules/ui/Widget.hpp"
#include "modules/core/InGame.hpp"
#include "modules/ui/Surface3x3.hpp"
#include "modules/io/Time.hpp"

namespace tt3d {
namespace Core {

using namespace tt3d;
using namespace tt3d::GL;
using namespace tt3d::UI;
using namespace tt3d::IO;

class TT3D: public IO::Application {
    private:
        IO::TimeFloat _updateInterval, _updateTime;
        SDL_Surface *window;
        
        ViewportHandle globalViewport;
        
        UI::GeometryBuffer *_uiBuffer;
        GenericGeometryBufferHandle _uiBufferHandle;
        UI::Material *_uiMaterial; // important to mark it as UI material here!
        UI::MaterialHandle _uiMaterialHandle;
        InGame *_rootWidget;
        WidgetHandle _rootHandle;
        Surface3x3 *_uiBackground;
        SurfaceHandle _uiBackgroundHandle;
        
        VFS::VFS *vfs;
    protected:
        virtual void initIO();
        virtual void initSDL();
        virtual void initGL();
        virtual void initUI();
        
        virtual void handleKeypress(const SDL_keysym &sym, const SDL_KeyActionMode mode);
        virtual void handleMouseButton(const SDL_MouseButtonEvent &button, const SDL_KeyActionMode mode);
        virtual void handleMouseMotion(const SDL_MouseMotionEvent &motion);
        
        virtual void freeGL();
        virtual void freeSDL();
        virtual void freeIO();
        virtual void freeUI();
    protected:
        virtual void freeApp();
        void doUpdate();
        virtual void perFrame(const IO::TimeFloat interval);
        virtual void initApp();
};

}
}

#endif
