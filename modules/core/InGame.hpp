#ifndef _TT3D_CORE_INGAME_H
#define _TT3D_CORE_INGAME_H

#include "modules/ui/RootWidget.hpp"
#include "modules/gl/Viewport.hpp"
#include "modules/gl/GeometryBuffer.hpp"
#include "modules/gl/Material.hpp"
#include "modules/gl/GeometryObject.hpp"
#include "modules/gl/Camera.hpp"
#include "modules/terrain/TerrainMesh.hpp"

namespace tt3d {
namespace Core {
    
using namespace tt3d;

typedef GL::GeometryBuffer<float, 3, 4> DebugGeometryBuffer;
    
class InGame: public UI::RootWidget {
    public:
        InGame(const GL::ViewportHandle viewport);
    private:
        GL::ViewportHandle _viewport;
    
        GL::GenericGeometryBufferHandle _debugBuffer;
        GL::MaterialHandle _debugMaterial;
        
        GL::GeometryObjectHandle _axis;
        GL::GeometryObjectHandle _grid;
        
        GL::CameraFreeSmooth *_camera;
        GL::CameraHandle _cameraHandle;
        bool _cameraMoved;
        
        Terrain::TerrainMesh *_mesh;
    private:
        void initAxis();
        void initCamera();
        void initGrid();
        void initTest();
    protected:
        virtual void doAbsRectChanged();
        virtual void doRenderCallback();
        virtual void doUpdateCallback(const IO::TimeFloat interval);
    public:
        virtual void handleKeypress(const SDL_keysym &sym, const IO::SDL_KeyActionMode mode, bool &handled);
        virtual void handleMouseButton(const SDL_MouseButtonEvent &button, const IO::SDL_KeyActionMode mode);
        virtual void handleMouseMotion(const SDL_MouseMotionEvent &motion);
};
    
}
}

#endif
