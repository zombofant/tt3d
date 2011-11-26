#include "Widget.hpp"

namespace tt3d {
namespace UI {

using namespace tt3d;


/* tt3d::UI::Widget */

Widget::Widget():
    _absRect(0, 0, 0, 0),
    _rect(0, 0, 0, 0),
    _children(new WidgetHandles()),
    _enabled(true),
    _visible(true),
    _invalidated(true),
    _flex(1),
    _flexSum(0),
    _name(""),
    _parent(),
    _animated(false)
{
    
}

void Widget::addChild(const WidgetHandle aWidget) {
    if (!doAcceptChild(aWidget)) {
        throw Error("Cannot make this a child of that.");
    }
    _children->push_back(aWidget);
    _flexSum += aWidget->getFlex();
    doAlign();
}

void Widget::doAbsRectChanged() {
    invalidate();
    doAlign();
}

bool Widget::doAcceptChild(const WidgetHandle aWidget) {
    return true;
}

bool Widget::doAcceptFocus() {
    return false;
}

void Widget::doAlign() {
    
}

WidgetHandle Widget::doHitTest(const Geometry::Point &aPoint) {
    WidgetHandle hit;
    if (!_absRect.contains(aPoint)) {
        return hit;
    }
    
    for (WidgetHandles::iterator it = _children->begin();
        it != _children->end();
        it++)
    {
        const WidgetHandle handle = *it;
        hit = handle->doHitTest(aPoint);
        if (hit.get())
            return hit;
    }
    return WidgetHandle(this);
}

void Widget::doKeypress(const SDL_keysym &sym, const IO::SDL_KeyActionMode mode, bool &handled) {
    
}

void Widget::doMouseButton(const SDL_MouseButtonEvent &button, const IO::SDL_KeyActionMode mode) {
    
}

void Widget::doMouseMotion(const SDL_MouseMotionEvent &motion) {
    
}

void Widget::doRelMetricsChanged() {
    
}

void Widget::doRenderBackground() {
    
}

void Widget::doRenderChildren() {
    for (WidgetHandles::iterator it = _children->begin();
        it != _children->end();
        it++)
    {
        const WidgetHandle handle = *it;
        handle->render();
    }
}

void Widget::doRenderForeground() {
    
}

void Widget::doUpdate(const double interval) {
    
}

void Widget::linkParent(WidgetHandle aParent) {
    
}

void Widget::removeChild(WidgetHandle aWidget) {
    WidgetHandles::iterator match = find(_children->begin(), _children->end(), aWidget);
    if (match != _children->end()) {
        _children->erase(match);
        if (aWidget->getParent().get() == this) {
            const WidgetHandle null;
            aWidget->setParent(null);
        }
    }
}

void Widget::setAbsRect(const Geometry::Rect rect) {
    if (rect == _absRect)
        return;
    _absRect = rect;
    doAbsRectChanged();
}

void Widget::setRelMetrics(const int32_t x, const int32_t y) {
    if ((_rect.x == x) && (_rect.y == y))
        return;
    _rect.x = x;
    _rect.y = y;
    doRelMetricsChanged();
}

void Widget::unlinkParent(WidgetHandle aParent) {
    WidgetHandle me(this);
    aParent->removeChild(me);
}

Geometry::Point Widget::clientToAbsolute(const Geometry::Point &point) {
    return Geometry::Point(point.x + _absRect.x, point.y + _absRect.y);
}

Geometry::Point Widget::clientToParent(const Geometry::Point &point) {
    return Geometry::Point(point.x + _rect.x, point.y + _rect.y);
}

void Widget::deleteChildren() {
    _children->clear();
}

void Widget::render() {
    doRenderBackground();
    doRenderChildren();
    doRenderForeground();
}

void Widget::update(const double interval) {
    if (_animated) {
        doUpdate(interval);
    } else if (_invalidated) {
        doUpdate(0.0);
    }
    _invalidated = false;
    
    for (WidgetHandles::iterator it = _children->begin();
        it != _children->end();
        it++)
    {
        const WidgetHandle handle = *it;
        handle->update(interval);
    }
}

void Widget::setParent(const WidgetHandle aValue) {
    WidgetHandle currentParent = _parent.lock();
    if (currentParent == aValue) {
        return;
    }
    if (currentParent.get()) {
        _parent.reset();
        unlinkParent(currentParent);
    }
    _parent = WidgetWeakHandle(aValue);
    if (aValue.get()) {
        linkParent(aValue);
    }
}

}
}
