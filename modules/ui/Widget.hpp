#ifndef _TT3D_UI_WIDGET_H
#define _TT3D_UI_WIDGET_H

#include <stdint.h>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <string>
#include <SDL/SDL.h>
#include "modules/io/SDL.hpp"
#include "modules/utils/Exception.hpp"
#include "modules/geometry/Rect.hpp"
#include "Surface.hpp"

namespace tt3d {
namespace UI {

using namespace tt3d;
using namespace tt3d::Geometry;


class Error: public Utils::Exception {
    public:
        Error(const std::string message): Utils::Exception(message) {};
        Error(const char* message): Utils::Exception(message) {};
        ~Error() throw() {};
};

class Widget;

typedef boost::shared_ptr<Widget> WidgetHandle;
typedef boost::weak_ptr<Widget> WidgetWeakHandle;
typedef std::vector<WidgetHandle> WidgetHandles;

class Widget {
    public:
        Widget();
    protected:
        Rect _absRect;
        Rect _rect;
        Rect _absClientRect;
        WidgetHandles *_children;
        bool _enabled, _visible, _invalidated;
        uint16_t _flex;
        uint32_t _flexSum;
        std::string _name;
        WidgetWeakHandle _parent;
        bool _animated;
        Borders _padding;
    protected:
        void addChild(const WidgetHandle aWidget);
        virtual void doAbsRectChanged();
        virtual void doAlign();
        virtual void doPaddingChanged();
        virtual void doRelMetricsChanged();
        virtual void doRenderBackground();
        void doRenderChildren();
        virtual void doRenderForeground();
        virtual void doSurfaceChanged();
        virtual void doUpdate(const double interval);
        void linkParent(WidgetHandle aParent);
        void removeChild(const WidgetHandle aWidget);
        void unlinkParent(WidgetHandle aParent);
        void updateClientRect();
        void updateFlexSum();
    public:
        virtual bool acceptsChild(const WidgetHandle aWidget);
        virtual bool acceptsFocus();
        Point clientToAbsolute(const Point &point);
        Point clientToParent(const Point &point);
        void deleteChildren();
        virtual void handleKeypress(const SDL_keysym &sym, const IO::SDL_KeyActionMode mode, bool &handled);
        virtual void handleMouseButton(const SDL_MouseButtonEvent &button, const IO::SDL_KeyActionMode mode);
        virtual void handleMouseMotion(const SDL_MouseMotionEvent &motion);
        virtual WidgetHandle hitTest(const Point &aPoint);
        void invalidate() { _invalidated = true; };
        void realign();
        void render();
        void update(const double interval);
    public:
        const Rect &getAbsRect() const { return _absRect; };
        bool getEnabled() const { return _enabled; };
        uint16_t getFlex() const { return _flex; };
        const std::string getName() const { return _name; };
        uint32_t getHeight() const { return _rect.h; };
        const Borders &getPadding() const { return _padding; };
        WidgetHandle getParent() const { return _parent.lock(); };
        const Rect &getRect() const { return _rect; };
        bool getVisible() const { return _visible; };
        uint32_t getWidth() const { return _rect.w; };
        
        void setAbsRect(const Rect rect);
        void setEnabled(const bool aValue) { _enabled = aValue; };
        void setFlex(const uint16_t aValue) { _flex = aValue; };
        void setHeight(const uint32_t aValue) { _rect.h = aValue; };
        void setName(const std::string aValue) { _name = std::string(aValue); };
        void setPadding(const Borders aValue);
        void setParent(WidgetHandle aValue);
        void setRelMetrics(const int32_t x, const int32_t y);
        void setVisible(const bool aValue) { _visible = aValue; };
        void setWidth(const uint32_t aValue) { _rect.w = aValue; };
        
};

}
}

#endif
