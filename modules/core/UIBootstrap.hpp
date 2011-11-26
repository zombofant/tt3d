#include <SDL/SDL.h>
#include "modules/io/SDL.hpp"
#include "modules/io/vfs/VFS.hpp"
#include "modules/io/vfs/MountDirectory.hpp"
#include "modules/gl/Viewport.hpp"
#include "modules/gl/GeometryBuffer.hpp"
#include "modules/gl/IndexBuffer.hpp"

namespace tt3d {
namespace Core {

using namespace tt3d;

class TT3D: public IO::Application {
    private:
        SDL_Surface *window;
        
        GL::Viewport *globalViewport;
        GL::GeometryBuffer<float, 2, 4> *testGeometry;
        GL::StaticIndexBuffer *testIndexBuffer;
        GL::IndexEntryHandle testIndexHandle;
        
        VFS::VFS *vfs;
    protected:
        virtual void initIO();
        virtual void initSDL();
        virtual void initGL();
        
        virtual void freeGL();
        virtual void freeSDL();
        virtual void freeIO();
    protected:
        virtual void freeApp();
        virtual void perFrame(const double interval);
        virtual void initApp();
};

}
}
