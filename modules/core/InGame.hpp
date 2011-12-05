/**********************************************************************
File name: InGame.hpp
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
