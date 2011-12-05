/**********************************************************************
File name: Rect.hpp
This file is part of: tt3d — Freeform transport simulation

LICENSE

The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations under
the License.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public license (the  "GPL License"), in which case  the
provisions of GPL License are applicable instead of those above.

FEEDBACK & QUESTIONS

For feedback and questions about tt3d please e-mail one of the authors
named in the AUTHORS file.
**********************************************************************/
#include <stdint.h>

#ifndef _TT3D_GEOMETRY_RECT_H
#define _TT3D_GEOMETRY_RECT_H

namespace tt3d {
namespace Math {

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

struct Borders {
    int32_t top, left, right, bottom;
    
    Borders();
    Borders(const int32_t all);
    Borders(const int32_t ahoriz, const int32_t avert);
    Borders(const int32_t atop, const int32_t aleft, const int32_t aright, const int32_t abottom);
    
    void setAll(const int32_t aValue);
    void setHorizontal(const int32_t aValue);
    void setVertical(const int32_t aValue);
    
    bool operator==(const Borders &other) const;
    Borders &operator=(const Borders &other);
};


}
}

#endif
