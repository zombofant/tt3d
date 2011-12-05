/**********************************************************************
File name: Boxes.hpp
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
#include "Widget.hpp"

namespace tt3d {
namespace UI {

using namespace tt3d;
using namespace tt3d::Math;

class Box: public Widget {
    protected:
        uint16_t _spacing;
    public:
        uint16_t getSpacing() { return _spacing; }
        void setSpacing(const uint16_t aValue);
};

class HBox: public Box {
    protected:
        virtual void doAlign();
};

class VBox: public Box {
    protected:
        virtual void doAlign();
};

}
}
