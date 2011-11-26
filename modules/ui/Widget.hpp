#include <stdint.h>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <string>
#include <SDL/SDL.h>
#include "modules/io/SDL.hpp"
#include "modules/utils/Exception.hpp"

namespace tt3d {
namespace UI {

using namespace tt3d;

struct Rect {
    int32_t x, y;
    uint32_t w, h;
    
    Rect(const int32_t ax, const int32_t ay, const uint32_t aw, const uint32_t ah);
};

struct Point {
    int32_t x, y;
    
    Point(const int32_t ax, const int32_t ay);
};

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
        virtual ~Widget();
    private:
        Rect _absRect;
        Rect _rect;
        WidgetHandles *_children;
        bool _enabled, _visible, _invalidated;
        uint16_t _flex;
        uint32_t _flexSum;
        std::string _name;
        WidgetWeakHandle _parent;
    protected:
        bool _animated;
    protected:
        void addChild(const WidgetHandle aWidget);
        virtual void doAbsRectChanged();
        virtual bool doAcceptChild(const WidgetHandle aWidget);
        virtual bool doAcceptFocus();
        virtual void doAlign();
        virtual void doHitTest(const Point &aPoint);
        virtual void doKeypress(const SDL_keysym &sym, const IO::SDL_KeyActionMode mode, bool &handled);
        virtual void doSurfaceChanged();
        virtual void doMouseButton(const SDL_MouseButtonEvent &button, const IO::SDL_KeyActionMode mode);
        virtual void doMouseMotion(const SDL_MouseMotionEvent &motion);
        virtual void doRelMetricsChanged();
        virtual void doRenderBackground();
        void doRenderChildren();
        virtual void doRenderForeground();
        virtual void doUpdate(const double interval);
        void removeChild(const WidgetHandle aWidget);
        void setAbsRect(const Rect rect);
        void setRelRect(const Rect rect);
    public:
        void deleteChildren();
        void invalidate() { _invalidated = true; };
        void render();
        virtual void update(const double interval);
    public:
        const Rect &getAbsRect() const { return _absRect; };
        bool getEnabled() const { return _enabled; };
        uint16_t getFlex() const { return _flex; };
        const std::string getName() const { return _name; };
        uint32_t getHeight() const { return _rect.h; };
        const WidgetWeakHandle getParent() const { return _parent; };
        const Rect &getRect() const { return _rect; };
        bool getVisible() const { return _visible; };
        uint32_t getWidth() const { return _rect.w; };
        
        void setEnabled(const bool aValue) { _enabled = aValue; };
        void setFlex(const uint16_t aValue) { _flex = aValue; };
        void setHeight(const uint32_t aValue) { _rect.h = aValue; };
        void setName(const std::string aValue) { _name = std::string(aValue); };
        void setParent(const WidgetHandle aValue);
        void setVisible(const bool aValue) { _visible = aValue; };
        void setWidth(const uint32_t aValue) { _rect.w = aValue; };
        
};

}
}
