#include <SDL/SDL.h>
#include <ctime>

namespace tt3d {
namespace SDL {
    
typedef enum {
    KM_PRESS,
    KM_RELEASE
} SDL_KeyActionMode;

timespec nanotime();
double timeIntervalToDouble(const timespec &prev, const timespec &curr);

class Application {
    public:
        Application();
        ~Application();
        
    private:
        bool terminated;
        timespec lastUpdate;
        uint32_t frameCount;
        timespec frameCounterStart;
        double currentFPS;
    
    protected:
        virtual void handleKeypress(const SDL_keysym &sym, const SDL_KeyActionMode mode);
        virtual void handleMouseButton(const SDL_MouseButtonEvent &button, const SDL_KeyActionMode mode);
        virtual void handleMouseMotion(const SDL_MouseMotionEvent &motion);
        virtual void handleUnhandledEvent(const SDL_Event &event);
        virtual void handleQuit();
        virtual void freeApp();
        virtual void initApp();
        virtual void perIteration();
        virtual void perFrame(const double interval);
        virtual void perSkippedFrame();
    
    public:
        void runApp();
        void terminate();
};

}
}
