#include "Widget.hpp"

namespace tt3d {
namespace UI {

using namespace tt3d;
using namespace tt3d::Math;


/* tt3d::UI::Widget */

Widget::Widget():
    _absRect(0, 0, 0, 0),
    _rect(0, 0, 0, 0),
    _absClientRect(0, 0, 0, 0),
    _children(new WidgetHandles()),
    _enabled(true),
    _visible(true),
    _invalidated(true),
    _flex(1),
    _flexSum(0),
    _name(""),
    _parent(),
    _animated(false),
    _padding(0, 0, 0, 0)
{
    
}

void Widget::addChild(const WidgetHandle aWidget) {
    if (!acceptsChild(aWidget)) {
        throw Error("Cannot make this a child of that.");
    }
    _children->push_back(aWidget);
    _flexSum += aWidget->getFlex();
}

void Widget::doAbsRectChanged() {
    invalidate();
}

void Widget::doAlign() {
    updateFlexSum();
    updateClientRect();
}

void Widget::doPaddingChanged() {
    invalidate();
}

void Widget::doRelMetricsChanged() {
    invalidate();
}

void Widget::doRenderBackground() {
    
}

/*void Widget::doRenderCallback() {
    
}*/

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

void Widget::doSurfaceChanged() {
    
}

void Widget::doUpdate(const TimeFloat interval) {
    
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

void Widget::setAbsRect(const Rect rect) {
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

void Widget::updateClientRect() {
    _absClientRect = Rect(
        _absRect.x + _padding.left,
        _absRect.y + _padding.top,
        _absRect.w - (_padding.left + _padding.right),
        _absRect.h - (_padding.top + _padding.bottom)
    );
}

void Widget::updateFlexSum() {
    uint32_t flexSum = 0;
    for (WidgetHandles::iterator it = _children->begin();
        it != _children->end();
        it++)
    {
        const WidgetHandle handle(*it);
        flexSum += handle->getFlex();
    }
    _flexSum = flexSum;
}

bool Widget::acceptsChild(const WidgetHandle aWidget) {
    return true;
}

bool Widget::acceptsFocus() {
    return false;
}

Point Widget::clientToAbsolute(const Point &point) {
    return Point(point.x + _absRect.x, point.y + _absRect.y);
}

Point Widget::clientToParent(const Point &point) {
    return Point(point.x + _rect.x, point.y + _rect.y);
}

void Widget::deleteChildren() {
    _children->clear();
}

void Widget::handleKeypress(const SDL_keysym &sym, const IO::SDL_KeyActionMode mode, bool &handled) {
    
}

void Widget::handleMouseButton(const SDL_MouseButtonEvent &button, const IO::SDL_KeyActionMode mode) {
    
}

void Widget::handleMouseMotion(const SDL_MouseMotionEvent &motion) {
    
}

WidgetHandle Widget::hitTest(const Point &aPoint) {
    WidgetHandle hit = WidgetHandle();
    if (!_absRect.contains(aPoint)) {
        return hit;
    }
    
    for (WidgetHandles::iterator it = _children->begin();
        it != _children->end();
        it++)
    {
        const WidgetHandle handle(*it);
        hit = WidgetHandle(handle->hitTest(aPoint));
        if (hit.get())
            return hit;
        else
            return handle;
    }
    // Appearantly we must not do WidgetHandle(this), as it causes the
    // widget to get garbage collected.
    return WidgetHandle();
}

void Widget::realign() {
    doAlign();
}

void Widget::render() {
    doRenderBackground();
    doRenderChildren();
    doRenderForeground();
}

void Widget::update(const TimeFloat interval) {
    if (_animated) {
        if (_invalidated) {
            doAlign();
        }
        doUpdate(interval);
    } else if (_invalidated) {
        doAlign();
        doUpdate(0.0);
    }
    _invalidated = false;
    
    for (WidgetHandles::iterator it = _children->begin();
        it != _children->end();
        it++)
    {
        const WidgetHandle handle(*it);
        handle->update(interval);
    }
}

void Widget::setPadding(const Borders aValue) {
    if (_padding == aValue)
        return;
    _padding = aValue;
    doPaddingChanged();
}

void Widget::setParent(const WidgetHandle aValue) {
    WidgetHandle currentParent(_parent.lock());
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
