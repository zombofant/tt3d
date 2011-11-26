#include "Widget.hpp"

namespace tt3d {
namespace UI {

using namespace tt3d;
using namespace tt3d::Geometry;

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
