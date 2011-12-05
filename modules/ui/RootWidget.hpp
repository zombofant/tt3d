/**********************************************************************
File name: RootWidget.hpp
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
#ifndef _TT3D_UI_TAB_WIDGET_H
#define _TT3D_UI_TAB_WIDGET_H

#include "Widget.hpp"
#include "Surface.hpp"
#include "modules/io/SDL.hpp"

namespace tt3d {
namespace UI {

using namespace tt3d;
using namespace tt3d::IO;

class RootWidget: public Widget {
    public:
        RootWidget();
    private:
        SurfaceHandle _backgroundSurface;
        GeometryObjectHandle _backgroundGeometry;
        WidgetHandle _focused;
        WidgetHandle _mouseGrabber;
        int _mouseGrabButton;
    protected:
        virtual void doAlign();
        virtual void doRenderBackground();
        void doRenderBackgroundGeometry();
        virtual void doRenderCallback();
        virtual void doUpdate(const TimeFloat interval);
        void doUpdateBackgroundGeometry();
        virtual void doUpdateCallback(const TimeFloat interval);
    public:
        virtual bool acceptsChild(const WidgetHandle aWidget);
        void deliverKeypress(const SDL_keysym &sym, const SDL_KeyActionMode mode);
        void deliverMouseButton(const SDL_MouseButtonEvent &event, const SDL_KeyActionMode mode);
        void deliverMouseMotion(const SDL_MouseMotionEvent &motion);
        virtual WidgetHandle hitTest(const Point &point);
    public:
        const SurfaceHandle getSurface() { return _backgroundSurface; }
        void setSurface(const SurfaceHandle surface);
};
    
}
}

#endif
