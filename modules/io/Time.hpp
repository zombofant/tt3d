#include <ctime>

namespace tt3d {
namespace IO {
    
typedef double TimeFloat;

timespec nanotime();
TimeFloat timeToDouble(const timespec &time);
TimeFloat timeIntervalToDouble(const timespec &prev, const timespec &curr);

}
}
