#include "Widget.hpp"

namespace tt3d {
namespace UI {

using namespace tt3d;
using namespace tt3d::Math;

class Button: public Widget {
    protected:
        virtual bool doAcceptChild(const WidgetHandle aWidget) { return false; }
        virtual bool doAcceptFocos() { return true; }
        virtual void doRenderBackground();
        virtual void doUpdate(const TimeFloat interval);
}

}
}
