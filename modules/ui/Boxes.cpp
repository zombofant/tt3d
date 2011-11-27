#include "Boxes.hpp"
#include <cmath>

namespace tt3d {
namespace UI {

using namespace tt3d;
using namespace tt3d::Geometry;

/* tt3d::UI::Box */

void Box::setSpacing(const uint16_t aValue) {
    if (_spacing == aValue) {
        return;
    }
    doAlign();
}

/* tt3d::UI::HBox */

void HBox::doAlign() {
    // this will update both the client rect and the flex sum
    Widget::doAlign();;
    int32_t x = _absClientRect.x;
    const int32_t childCount = (int32_t)(_children->size());
    if (childCount == 0) {
        return;
    }
    
    int32_t freeSpace = _absClientRect.w - (childCount - 1) * _spacing;
    const double spacePerFlex = (double)(freeSpace) / _flexSum;
    
    for (WidgetHandles::iterator it = _children->begin();
        it != _children->end();
        it++)
    {
        WidgetHandle handle = *it;
        const int32_t w = round(spacePerFlex * handle->getFlex());
        int32_t h = handle->getHeight();
        if ((h > _rect.h) || (h <= 0)) {
            h = _rect.h;
        }
        handle->setAbsRect(Rect(x, _absRect.y, w, h));
        x += w + _spacing;
    }
}

/* tt3d::UI::VBox */

void VBox::doAlign() {
    // this will update both the client rect and the flex sum
    Widget::doAlign(); 
    int32_t y = _absRect.y + _padding.top;
    const int32_t childCount = (int32_t)(_children->size());
    if (childCount == 0) {
        return;
    }
    
    int32_t freeSpace = _absClientRect.h - (childCount - 1) * _spacing;
    const double spacePerFlex = (double)(freeSpace) / _flexSum;
    
    for (WidgetHandles::iterator it = _children->begin();
        it != _children->end();
        it++)
    {
        WidgetHandle handle = *it;
        const int32_t h = round(spacePerFlex * handle->getFlex());
        int32_t w = handle->getHeight();
        if ((w > _rect.w) || (w <= 0)) {
            w = _rect.w;
        }
        handle->setAbsRect(Rect(_absRect.x, y, w, h));
        y += h + _spacing;
    }
}

}
}
