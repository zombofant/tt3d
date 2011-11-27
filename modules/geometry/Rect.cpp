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

/* tt3d::UI::Borders */

Borders::Borders():
    top(0),
    left(0),
    right(0),
    bottom(0)
{
    
}

Borders::Borders(const int32_t all):
    top(all),
    left(all),
    right(all),
    bottom(all)
{
    
}

Borders::Borders(const int32_t ahoriz, const int32_t avert):
    top(avert),
    left(ahoriz),
    right(ahoriz),
    bottom(avert)
{
    
}

Borders::Borders(const int32_t atop, const int32_t aleft, const int32_t aright, const int32_t abottom):
    top(atop),
    left(aleft),
    right(aright),
    bottom(abottom)
{
    
}

void Borders::setAll(const int32_t aValue) {
    top = aValue;
    left = aValue;
    right = aValue;
    bottom = aValue;
}

void Borders::setHorizontal(const int32_t aValue) {
    left = aValue;
    right = aValue;
}

void Borders::setVertical(const int32_t aValue) {
    top = aValue;
    bottom = aValue;
}

bool Borders::operator==(const Borders &other) const {
    return ((left == other.left) && (top == other.top) && (right == other.right) && (bottom == other.bottom));
}

Borders &Borders::operator=(const Borders &other) {
    top = other.top;
    left = other.left;
    right = other.right;
    bottom = other.bottom;
    return *this;
}

}
}
