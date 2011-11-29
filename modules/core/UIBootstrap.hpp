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

namespace tt3d {
namespace Core {

using namespace tt3d;
using namespace tt3d::GL;
using namespace tt3d::UI;
using namespace tt3d::IO;

class TT3D: public IO::Application {
    private:
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
        virtual void perFrame(const double interval);
        virtual void initApp();
};

}
}

#endif
