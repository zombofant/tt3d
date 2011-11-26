#include "Widget.hpp"

namespace tt3d {
namespace UI {

using namespace tt3d;

/* tt3d::UI::Rect */

Rect::Rect(const int32_t ax, const int32_t ay, const uint32_t aw, const uint32_t ah):
    x(ax),
    y(ay),
    w(aw),
    h(ah)
{
    
}

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
}

}
}
