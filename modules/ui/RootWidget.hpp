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
        virtual void doUpdate(const double interval);
        void doUpdateBackgroundGeometry();
        virtual void doUpdateCallback(const double interval);
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
