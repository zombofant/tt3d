#ifndef _TT3D_UI_TAB_WIDGET_H
#define _TT3D_UI_TAB_WIDGET_H

#include "Widget.hpp"
#include "Surface.hpp"

namespace tt3d {
namespace UI {
    
class RootWidget: public Widget {
    public:
        RootWidget();
    private:
        const SurfaceHandle _backgroundSurface;
        GeometryObjectHandle _backgroundGeometry;
    protected:
        virtual WidgetHandle doHitTest(const Point &point);
        virtual void doRenderBackground();
        virtual void doUpdate(const double interval);
    public:
        const SurfaceHandle getSurface() { return _backgroundSurface; }
        void setSurface(const SurfaceHandle surface);
};
    
}
}

#endif
