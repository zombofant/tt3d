#include "Rect.hpp"

namespace tt3d {
namespace Geometry {

/* tt3d::UI::Point */

Point::Point(const int32_t ax, const int32_t ay):
    x(ax),
    y(ay)
{
    
}

/* tt3d::UI::Rect */

Rect::Rect(const int32_t ax, const int32_t ay, const uint32_t aw, const uint32_t ah):
    x(ax),
    y(ay),
    w(aw),
    h(ah)
{
    
}

bool Rect::contains(const Point &aPoint) const {
    return ((aPoint.x >= x) && (aPoint.x < x + w) && (aPoint.y >= y) && (aPoint.y < y + h));
}

bool Rect::operator==(const Rect &other) const {
    return (x == other.x) && (y == other.y) && (w == other.w) && (h == other.h);
}

Rect &Rect::operator+=(const Point &by) {
    x += by.x;
    y += by.y;
    return *this;
}

Rect Rect::operator+(const Point &by) const {
    return Rect(x + by.x, y + by.y, w, h);
}

}
}
