#include "RootWidget.hpp"
#include <GL/glew.h>

namespace tt3d {
namespace UI {
    
using namespace tt3d;
using namespace tt3d::Geometry;
using namespace tt3d::IO;

SDL_MouseButtonEvent transposedButtonEvent(const SDL_MouseButtonEvent &button, const Rect &rect) {
    SDL_MouseButtonEvent transposed = button;
    transposed.x -= rect.x;
    transposed.y -= rect.y;
    return transposed;
}

SDL_MouseMotionEvent transposedMotionEvent(const SDL_MouseMotionEvent &motion, const Rect &rect) {
    SDL_MouseMotionEvent transposed = motion;
    transposed.x -= rect.x;
    transposed.y -= rect.y;
    return transposed;
}

/* tt3d::UI::RootWidget */

RootWidget::RootWidget():
    Widget(),
    _backgroundSurface(),
    _backgroundGeometry(),
    _focused(),
    _mouseGrabber(),
    _mouseGrabButton(0)
{
    _animated = true;
}

void RootWidget::doAlign() {
    for (WidgetHandles::iterator it = _children->begin();
        it != _children->end();
        it++)
    {
        const WidgetHandle handle = *it;
        handle->setAbsRect(_absRect);
    }
}

void RootWidget::doRenderBackground() {
    doRenderCallback();
    doRenderBackgroundGeometry();
}

void RootWidget::doRenderBackgroundGeometry() {
    GeometryObject *background = _backgroundGeometry.get();
    if (background) {
        background->drawDirect(GL_TRIANGLES);
    }
}

void RootWidget::doRenderCallback() {
    
}

void RootWidget::doUpdate(const double interval) {
    doUpdateCallback(interval);
    doUpdateBackgroundGeometry();
}

void RootWidget::doUpdateBackgroundGeometry() {
    if (_invalidated) {
        Surface *surface = _backgroundSurface.get();
        if (surface) {
            surface->updateGeometry(_absRect, _backgroundGeometry);
        }
    }
}

void RootWidget::doUpdateCallback(const double interval) {
    
}

bool RootWidget::acceptsChild(const WidgetHandle aWidget) {
    return true;
}

void RootWidget::deliverKeypress(const SDL_keysym &sym, const SDL_KeyActionMode mode) {
    bool handled;
    handled = false;
    if (_focused.get()) {
        _focused->handleKeypress(sym, mode, handled);
    }
    if (!handled) {
        handleKeypress(sym, mode, handled);
    }
}

void RootWidget::deliverMouseButton(const SDL_MouseButtonEvent &button, const SDL_KeyActionMode mode) {
    if (_mouseGrabber.get()) {
        const Rect grabAbs = _mouseGrabber->getAbsRect();
        SDL_MouseButtonEvent transposed = transposedButtonEvent(button, grabAbs);
        _mouseGrabber->handleMouseButton(transposed, mode);
        if ((button.button == _mouseGrabButton) && (mode == KM_RELEASE)) {
            _mouseGrabber = WidgetHandle();
        }
        return;
    }
    WidgetHandle hit(hitTest(Point(button.x, button.y)));
    if (button.button == 1 && hit.get()) {
        if (hit->acceptsFocus()) {
            _focused = hit;
        }
    }
    if (hit.get()) {
        if (!_mouseGrabber.get() && mode == KM_PRESS) {
            _mouseGrabber = hit;
            _mouseGrabButton = button.button;
        }
        const Rect hitAbs = hit->getAbsRect();
        SDL_MouseButtonEvent transposed = transposedButtonEvent(button, hitAbs);
        hit->handleMouseButton(transposed, mode);
    } else {
        handleMouseButton(button, mode);
    }
}

void RootWidget::deliverMouseMotion(const SDL_MouseMotionEvent &motion) {
    if (_mouseGrabber.get()) {
        const Rect grabAbs = _mouseGrabber->getAbsRect();
        SDL_MouseMotionEvent transposed = transposedMotionEvent(motion, grabAbs);
        _mouseGrabber->handleMouseMotion(transposed);
    } else {
        const Point p = Point(motion.x, motion.y);
        std::cout << "hittest start" << std::endl;
        WidgetHandle hit(hitTest(p));
        std::cout << "hittest done" << std::endl;
        if (hit.get()) {
            const Rect hitAbs = hit->getAbsRect();
            SDL_MouseMotionEvent transposed = transposedMotionEvent(motion, hitAbs);
            hit->handleMouseMotion(transposed);
        } else {
            handleMouseMotion(motion);
        }
    }
}

WidgetHandle RootWidget::hitTest(const Point &point) {
    std::cout << "hittest" << std::endl;
    if (_invalidated)
        updateClientRect();
    WidgetHandle result(Widget::hitTest(point));
    if (result.get() == this) {
        //const Point client = Point(point.x - _absRect.x, point.y - _absRect.y);
        //if ((client.x > _leftMargin) && (client.y > _topMargin) && (client.x < 
        if (_absClientRect.contains(point)) {
            return WidgetHandle();
        }
    }
    return result;
}

void RootWidget::setSurface(const SurfaceHandle surface) {
    _backgroundSurface = surface;
}

}
}
