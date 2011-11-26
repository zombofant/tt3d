#include <stdint.h>

#ifndef _TT3D_GEOMETRY_RECT_H
#define _TT3D_GEOMETRY_RECT_H

namespace tt3d {
namespace Geometry {

struct Point {
    int32_t x, y;
    
    Point(const int32_t ax, const int32_t ay);
};

struct Rect {
    int32_t x, y;
    int32_t w, h;
    
    Rect(const int32_t ax, const int32_t ay, const uint32_t aw, const uint32_t ah);
    
    bool contains(const Point &aPoint) const;
    
    bool operator==(const Rect &other) const;
    Rect &operator+=(const Point &by);
    Rect operator+(const Point &by) const;
};


}
}

#endif
