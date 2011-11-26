#include <cstddef>

namespace tt3d {
namespace Utils {

class BufferMap {
    public:
        virtual size_t map(const size_t index) = 0; 
};

}
}
