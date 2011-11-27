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

namespace tt3d {
namespace Core {

using namespace tt3d;

class TT3D: public IO::Application {
    private:
        SDL_Surface *window;
        
        GL::ViewportHandle globalViewport;
        
        InGame *_rootWidget;
        UI::WidgetHandle _rootHandle;
        UI::Material *_uiMaterial;
        GL::MaterialHandle _uiMaterialHandle;
        UI::Surface3x3 *_uiBackground;
        GL::SurfaceHandle _uiBackgroundHandle;
        
        VFS::VFS *vfs;
    protected:
        virtual void initIO();
        virtual void initSDL();
        virtual void initGL();
        virtual void initUI();
        
        virtual void freeGL();
        virtual void freeSDL();
        virtual void freeIO();
        virtual void freeUI();
    protected:
        virtual void freeApp();
        virtual void perFrame(const double interval);
        virtual void initApp();
};

}
}

#endif
