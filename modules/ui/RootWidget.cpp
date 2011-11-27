#include "RootWidget.hpp"
#include <GL/glew.h>

namespace tt3d {
namespace UI {
    
using namespace tt3d;
using namespace tt3d::Geometry;

/* tt3d::UI::RootWidget */

RootWidget::RootWidget():
    Widget(),
    _backgroundSurface(),
    _backgroundGeometry()
{
    _padding = Borders(16, 24, 2, 2);
}

WidgetHandle RootWidget::doHitTest(const Point &point) {
    if (_invalidated)
        updateClientRect();
    WidgetHandle result = Widget::doHitTest(point);
    if (result.get() == this) {
        //const Point client = Point(point.x - _absRect.x, point.y - _absRect.y);
        //if ((client.x > _leftMargin) && (client.y > _topMargin) && (client.x < 
        if (_absClientRect.contains(point)) {
            return WidgetHandle();
        }
    }
    return result;
}

void RootWidget::doRenderBackground() {
    GeometryObject *background = _backgroundGeometry.get();
    if (background) {
        background->drawDirect(GL_TRIANGLES);
    }
}

void RootWidget::doUpdate(const double interval) {
    Widget::doUpdate(interval);
    Surface *surface = _backgroundSurface.get();
    if (surface) {
        surface->updateGeometry(_absRect, _backgroundGeometry);
    }
}

}
}
