#include <SDL/SDL.h>
#include "modules/io/SDL.hpp"
#include "modules/io/vfs/VFS.hpp"
#include "modules/io/vfs/MountDirectory.hpp"

namespace tt3d {
namespace Core {

using namespace tt3d;

class TT3D: public SDL::Application {
    private:
        SDL_Surface *window;
        
        VFS::VFS vfs;
    protected:
        virtual void initSDL();
        virtual void initGL();
        
        virtual void freeGL();
        virtual void freeSDL();
    protected:
        virtual void freeApp();
        virtual void perFrame(const double interval);
        virtual void initApp();
};

}
}
